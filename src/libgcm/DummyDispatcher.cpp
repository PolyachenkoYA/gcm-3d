#include "libgcm/DummyDispatcher.hpp"
#include "libgcm/Engine.hpp"

#include "libgcm/Body.hpp"

using namespace gcm;
using std::logic_error;
using std::string;

DummyDispatcher::DummyDispatcher() {
    INIT_LOGGER("gcm.Dispatcher");
    outlines = NULL;
    outlinesNum = -1;
}

DummyDispatcher::~DummyDispatcher() {
    if( outlines != NULL )
        delete[] outlines;
}

void DummyDispatcher::prepare(int numberOfWorkers, AABB* scene)
{
    auto& engine = Engine::getInstance();

    myNumberOfWorkers =  numberOfWorkers;
    if( numberOfWorkers > 1 && numberOfWorkers < engine.getNumberOfBodies() )
        throw logic_error("You should have either 1 worker OR more workers than bodies. It's crazy stupid internal dispatcher limitation. We are really sorry.");

    LOG_DEBUG("Start preparation for " << numberOfWorkers << " workers");
    assert_gt(numberOfWorkers, 0 );

    if( outlines != NULL )
        delete[] outlines;
    outlines = new AABB[numberOfWorkers];
    outlinesNum = numberOfWorkers;
    rank = engine.getRank();

    if( numberOfWorkers == 1)
    {
        outlines[rank] = *scene;
        myBodyId = "__any";
        LOG_DEBUG("Preparation done");
        return;
    }

    int numberOfBodies = engine.getNumberOfBodies();
    LOG_DEBUG("Start preparation for " << numberOfBodies << " bodies");
    workersPerBody = new int[numberOfBodies];

    float totalNodes = 0;
    for( int i = 0; i < numberOfBodies; i++ )
    {
        totalNodes += getBodyNodesNumber(engine.getBody(i)->getId());
        LOG_DEBUG("NumberOfNodes[" << i <<"]: " << getBodyNodesNumber(engine.getBody(i)->getId()));
    }
    LOG_DEBUG("Total nodes: " << totalNodes);
    float nodesPerProcess = totalNodes / numberOfWorkers;
    LOG_DEBUG("Nodes per process: " << nodesPerProcess);

    for( int i = 0; i < numberOfBodies; i++ )
    {
        workersPerBody[i] = (int)( getBodyNodesNumber(engine.getBody(i)->getId()) / nodesPerProcess );
        if( workersPerBody[i] == 0 )
            workersPerBody[i] = 1;
    }

    while( distributionIsOk() != 0 )
    {
        adjustDistribution();
    }

    for( int i = 0; i < numberOfBodies; i++ )
        LOG_DEBUG("Workers for body " << i << ": " << workersPerBody[i]);

    int workersPassed = 0;
    int bodyNum = 0;
    while( workersPassed + workersPerBody[bodyNum] < rank + 1 )
    {
        workersPassed += workersPerBody[bodyNum];
        bodyNum++;
    }
    myBodyId = engine.getBody(bodyNum)->getId();
    LOG_DEBUG("Scheduled to work with body: " << myBodyId);

    int slicingDirection = getBodySlicingDirection(myBodyId);
    AABB myScene = getBodyOutline(myBodyId);
    float h = ( myScene.max_coords[slicingDirection] - myScene.min_coords[slicingDirection] ) / workersPerBody[bodyNum];
    LOG_DEBUG("Slicing direction: " << slicingDirection << " Slice: " << h);

    int i = rank - workersPassed;
    for( int j = 0; j < 3; j++ )
    {
        if( j != slicingDirection )
        {
            outlines[rank].min_coords[j] = myScene.min_coords[j];
            outlines[rank].max_coords[j] = myScene.max_coords[j];
        }
        else
        {
            outlines[rank].min_coords[j] = myScene.min_coords[j] + i * h;
            outlines[rank].max_coords[j] = myScene.min_coords[j] + (i+1) * h;;
        }
    }
    LOG_DEBUG("Preparation done");

    delete[] workersPerBody;
}

int DummyDispatcher::distributionIsOk()
{
    int totalNum = 0;
    for( int i = 0; i < Engine::getInstance().getNumberOfBodies(); i++ )
        totalNum += workersPerBody[i];
    if( totalNum > myNumberOfWorkers )
        return 1;
    else if( totalNum < myNumberOfWorkers )
        return -1;
    else
        return 0;
}

int DummyDispatcher::getLeastComputedBody()
{
    int num = 0;
    for( int i = 0; i < Engine::getInstance().getNumberOfBodies(); i++ )
        if( workersPerBody[num] > workersPerBody[i] )
            num = i;
    return num;
}

int DummyDispatcher::getMostComputedBody()
{
    int num = 0;
    for( int i = 0; i < Engine::getInstance().getNumberOfBodies(); i++ )
        if( workersPerBody[num] < workersPerBody[i] )
            num = i;
    return num;
}

void DummyDispatcher::adjustDistribution()
{
    int res = distributionIsOk();
    if( res > 0 )
        workersPerBody[ getMostComputedBody() ]--;
    if( res < 0 )
        workersPerBody[ getLeastComputedBody() ]++;
}

bool DummyDispatcher::isMine(float coords[3], string bodyId) {
    return ( getOwner( coords[0], coords[1], coords[2], bodyId ) == rank );
}

bool DummyDispatcher::isMine(double coords[3], string bodyId) {
    return ( getOwner( coords[0], coords[1], coords[2], bodyId ) == rank );
}

int DummyDispatcher::getOwner(float coords[3], string bodyId)
{
    return getOwner( coords[0], coords[1], coords[2], bodyId );
}

int DummyDispatcher::getOwner(float x, float y, float z, string bodyId)
{
    if( Engine::getInstance().getNumberOfWorkers() != 1 && bodyId != myBodyId )
        return -1;
    for( int i = 0; i < outlinesNum; i++ )
    {
        if( outlines[i].isInAABB(x+dX,y+dY,z+dZ) )
            return i;
    }
    return -1;
}

int DummyDispatcher::getOwner(float coords[3])
{
    return getOwner( coords[0], coords[1], coords[2] );
}

int DummyDispatcher::getOwner(float x, float y, float z)
{
    for( int i = 0; i < outlinesNum; i++ )
    {
        if( outlines[i].isInAABB(x+dX,y+dY,z+dZ) )
            return i;
    }
    return -1;
}

AABB* DummyDispatcher::getOutline(int index)
{
    assert_ge(index, 0);
    assert_lt(index, outlinesNum);
    return outlines + index;
}

void DummyDispatcher::printZones()
{
    for( int i = 0; i < outlinesNum; i++ )
        LOG_DEBUG("Zone " << i << ": " << outlines[i]);
    LOG_DEBUG("My space shift: [" << dX << "; " << dY << "; " << dZ << "]");
}

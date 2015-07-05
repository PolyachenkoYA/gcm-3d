#include "libgcm/BruteforceCollisionDetector.hpp"

#include "libgcm/node/CalcNode.hpp"
#include "libgcm/Engine.hpp"

using namespace gcm;
using std::string;
using std::vector;

BruteforceCollisionDetector::BruteforceCollisionDetector() {
    INIT_LOGGER("gcm.BruteforceCollisionDetector");
}

BruteforceCollisionDetector::~BruteforceCollisionDetector() {
}

string BruteforceCollisionDetector::getType () const
{
    return "BruteforceCollisionDetector";
}

void BruteforceCollisionDetector::find_collisions(vector<CalcNode> &virt_nodes)
{
    Engine& e = Engine::getInstance();
    AABB intersection;
    //data_bus->sync_outlines();
    vector<CalcNode> local_nodes;

    LOG_INFO("Processing local/local collisions with threshold " << get_threshold());

    // process collisions between local nodes and local faces
    // we start both cycles from zero because collision should be 'symmetric'
    for (int i = 0; i < e.getNumberOfBodies(); i++)
    {
        for (int j = 0; j < e.getNumberOfBodies(); j++) 
        {
            Mesh* mesh1 = e.getBody(i)->getMeshes();
            Mesh* mesh2 = e.getBody(j)->getMeshes();
            AABB outl1 = mesh1->getExpandedOutline();
            AABB outl2 = mesh2->getExpandedOutline();

            LOG_INFO("Local mesh #" << mesh1->getId() << " with outline " << outl1
                      << " and local mesh #" << mesh2->getId() << " with outline " << outl2);

            if ((i != j) && (find_intersection(outl1, outl2, intersection))) 
            {
                LOG_INFO("Collision detected between local mesh zone #" << mesh1->getId()
                          << " and local mesh zone #" << mesh2->getId() << ". "
                          << "Intersection: " << intersection);
                // find local nodes inside intersection
                find_nodes_in_intersection(mesh1, intersection, local_nodes);
                LOG_INFO("Got " << local_nodes.size() << " nodes");

                LOG_INFO("Virt nodes size before processing: " << virt_nodes.size());

                // process collisions
                float direction[3];
                for (unsigned int k = 0; k < local_nodes.size(); k++) 
                {
                    if (k % 10000 == 0)
                        LOG_INFO("Nodes processed: " << k);

                    // Check axis directions
                    for (int m = 0; m < 3; m++) 
                    {
                        mesh1->findBorderNodeNormal(local_nodes[k],
                                        &direction[0], &direction[1], &direction[2], false);

                        /*if (direction[m] > 0)
                            direction[m] = 1;
                        else
                            direction[m] = -1;
                        for (int z = 0; z < 3; z++)
                            if (z != m)
                                direction[z] = 0;*/

                        CalcNode new_node;
                        if( mesh2->interpolateBorderNode(
                                local_nodes[k].coords[0], local_nodes[k].coords[1], local_nodes[k].coords[2],
                                direction[0] * get_threshold(), direction[1] * get_threshold(), direction[2] * get_threshold(), new_node) )
                        {
                            new_node.setIsBorder(true);
                            new_node.setInContact(true);
                            new_node.contactNodeNum = j;
                            new_node.setCustomFlag(CalcNode::FLAG_1, 1);
                            (mesh1->getNode(local_nodes[k].number)).setInContact(true);
                            (mesh1->getNode(local_nodes[k].number)).contactNodeNum = virt_nodes.size();
                            (mesh1->getNode(local_nodes[k].number)).contactDirection = m;
                            virt_nodes.push_back(new_node);
                            break;
                        }
                    }
                }
            }

            LOG_INFO("Virt nodes size after processing: " << virt_nodes.size());

            // clear
            local_nodes.clear();
        }
    }
    
    LOG_INFO("Local/local collisions processed");

}

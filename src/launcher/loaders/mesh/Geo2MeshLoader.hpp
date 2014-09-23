#ifndef LAUNCHER_GEO2MESHLOADER_H
#define LAUNCHER_GEO2MESHLOADER_H

#include <string>

#include "libgcm/GCMDispatcher.hpp"
#include "libgcm/util/AABB.hpp"
#include "libgcm/util/Singleton.hpp"
#include "libgcm/mesh/tetr/TetrMeshSecondOrder.hpp"
#include "libgcm/mesh/tetr/Geo2MeshLoader.hpp"

#include "launcher/util/xml.hpp"

#include "launcher/loaders/mesh/MeshLoader.hpp"

using namespace gcm;
using namespace std;

namespace launcher
{
    class Geo2MeshLoader: public Singleton<launcher::Geo2MeshLoader>, public launcher::MeshLoader<TetrMeshSecondOrder>
    {
        protected:
            void parseDesc(const xml::Node& desc, string& id, string& fileName, float& tetrSize);
            void loadMesh(const xml::Node& desc, TetrMeshSecondOrder* mesh) override;
        public:
            void preLoadMesh(const xml::Node& desc, AABB& aabb, int& sliceDirection, int& numberOfNodes);

            const static string MESH_TYPE;
    };
}
        
#endif

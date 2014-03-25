#include "AnisotropicElasticMaterialLoader.h"
#include "materials/IAnisotropicElasticMaterial.h"

#include <cstdlib>
#include <cerrno>
#include <utility>
#include <sstream>
#include <boost/lexical_cast.hpp>

#include "materials/IAnisotropicElasticMaterial.h"

using boost::lexical_cast;

const string launcher::AnisotropicElasticMaterialLoader::RHEOLOGY_TYPE = "AnisotropicElastic";

gcm::AnisotropicElasticMaterial* launcher::AnisotropicElasticMaterialLoader::load(xml::Node desc)
{
    AttrList attrs = desc.getAttributes();

    string name = getAttributeByName(attrs, "name");
    string rheology = getAttributeByName(attrs, "rheology");
    assert(rheology == RHEOLOGY_TYPE);

    IAnisotropicElasticMaterial::RheologyParameters params;

    gcm_real rho = lexical_cast<gcm_real>(desc.getChildByName("rho").getTextContent());
    gcm_real crackThreshold = lexical_cast<gcm_real>(desc.getChildByName("crackThreshold").getTextContent());

    int k = 0;
    for (int i = 1; i <= 6; i++)
        for (int j = i; j <= 6; j++) {
            stringstream cxx;
            cxx << "c" << i << j;
            params.values[k++] = lexical_cast<gcm_real>(desc.getChildByName(cxx.str()).getTextContent());
        }

    if (rho <= 0.0)
        THROW_INVALID_INPUT("Seems xml snippet does not contain valid rheology parameters.");

    for (k = 0; k < ANISOTROPIC_ELASTIC_MATERIALS_PARAMETERS_NUM; k++)
        if (params.values[k] < 0.0)
            THROW_INVALID_INPUT("Seems xml snippet does not contain valid rheology parameters.");

    return new AnisotropicElasticMaterial(name, rho, crackThreshold, params);
}
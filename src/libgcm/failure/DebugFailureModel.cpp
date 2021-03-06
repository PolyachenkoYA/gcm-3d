#include "libgcm/failure/DebugFailureModel.hpp"

using namespace gcm;

DebugFailureModel::DebugFailureModel() {
    type = "DebugFailureModel";
    INIT_LOGGER( "gcm.DebugFailureModel" );
    criteria.push_back(new MaxStressContinualFailureCriterion());
    correctors.push_back(new ZeroDeviatorFailedMaterialCorrector());
}

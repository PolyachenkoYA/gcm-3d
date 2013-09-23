#ifndef _GCM_EXTERNAL_FORCE_CALCULATOR_H
#define _GCM_EXTERNAL_FORCE_CALCULATOR_H  1

#include "BorderCalculator.h"
#include <gsl/gsl_linalg.h>
#include "../../Math.h"

class ExternalForceCalculator : public BorderCalculator
{
public:
	ExternalForceCalculator();
	~ExternalForceCalculator();
	void do_calc(CalcNode* cur_node, CalcNode* new_node, ElasticMatrix3D* matrix, float* values[], bool inner[], float outer_normal[], float scale);
	inline string getType() {
		return "ExternalForceCalculator";
	}
	void set_parameters(float sn, float st, float xv, float yv, float zv);

protected:

private:
	float normal_stress;
	float tangential_stress;
	float tangential_direction[3];

	// Used for border calculation
	gsl_matrix *U_gsl;
	gsl_vector *om_gsl;
	gsl_vector *x_gsl;
	gsl_permutation *p_gsl;
};

#endif
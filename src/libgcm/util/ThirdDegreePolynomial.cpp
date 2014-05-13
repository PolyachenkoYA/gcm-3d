#include "libgcm/util/ThirdDegreePolynomial.hpp"


gcm::ThirdDegreePolynomial::ThirdDegreePolynomial(const gcm_matrix &A, int stage) {
	if (stage == 0) {
		double r = A(0,3);
		a = r * (-A(5,2) - A(4,1) - A(3,0));
		b = r * r * ((A(4,1) + A(3,0)) * A(5,2) - A(4,2) * A(5,1) - 
				A(3,2) * A(5,0) + A(3,0) * A(4,1) - A(3,1) * A(4,0));
		c = r * r * r * ((-A(3,0) * A(4,1) + A(3,1) * A(4,0)) * A(5,2) + 
				(A(3,0) * A(4,2) - A(3,2) * A(4,0)) * A(5,1) +
				(-A(3,1) * A(4,2) + A(3,2) * A(4,1)) * A(5,0));
	} else if (stage == 1) {
		double r = A(0,4);
		a = r * (-A(7,2) - A(6,1) - A(4,0));
		b = r * r * ((A(6,1) + A(4,0)) * A(7,2) - A(6,2) * A(7,1) - 
				A(4,2) * A(7,0) + A(4,0) * A(6,1) - A(4,1) * A(6,0));
		c = r * r * r * ((-A(4,0) * A(6,1) + A(4,1) * A(6,0)) * A(7,2) + 
				(A(4,0) * A(6,2) - A(4,2) * A(6,0)) * A(7,1) +
				(-A(4,1) * A(6,2) + A(4,2) * A(6,1)) * A(7,0));
	} else if (stage == 2) {
		double r = A(0,5);
		a = r * (-A(8,2) - A(7,1) - A(5,0));
		b = r * r * ((A(7,1) + A(5,0)) * A(8,2) - A(7,2) * A(8,1) - 
				A(5,2) * A(8,0) + A(5,0) * A(7,1) - A(5,1) * A(7,0));
		c = r * r * r * ((-A(5,0) * A(7,1) + A(5,1) * A(7,0)) * A(8,2) + 
				(A(5,0) * A(7,2) - A(5,2) * A(7,0)) * A(8,1) +
				(-A(5,1) * A(7,2) + A(5,2) * A(7,1)) * A(8,0));
	} else {
		THROW_BAD_CONFIG("Wrong stage number");
	}
    findRoots();
};

void gcm::ThirdDegreePolynomial::findRoots()
{
    isMltpl = false;
    long double Q = (a*a - 3*b)/9;
    long double R = (2*a*a*a - 9*a*b + 27*c)/54;
    if ( abs (R / sqrt(Q*Q*Q)) > 1 - 1e-10*EQUALITY_TOLERANCE ) {
        // Two roots are equal
        isMltpl = true;
        if (R > 0) {
            roots[0] = - sqrt(Q) - a/3;
            roots[1] = roots[2] = 2 * sqrt(Q) - a/3;
        } else {
            roots[0] = 2 * sqrt(Q) - a/3;
            roots[1] = roots[2] = - sqrt(Q) - a/3;
        }
    } else {
        // All the roots seem to be different
        double phi = acos (R / sqrt(Q * Q * Q)) / 3;
        roots[0] = -2 * sqrt(Q) * cos (phi) - a/3;
        roots[1] = -2 * sqrt(Q) * cos (phi + 2 * M_PI / 3) - a / 3;
        roots[2] = -2 * sqrt(Q) * cos (phi - 2 * M_PI / 3) - a / 3;
        // Or two ones are still equal?
        for (int i = 0; i < 2; i++)
            for (int j = i+1; j < 3; j++)
                if ( abs (roots[i] - roots[j]) / max (roots[i], roots[j]) < 1e-2 ) {
                    // k  =  no i  and  no j
                    int k = !i;
                    k += (k == j) ? 1 : 0;
                    if ( abs(roots[i] - roots[k]) / max(roots[i], roots[k]) < 1e-2 )
                        THROW_INVALID_INPUT("All the roots are equal");
                    isMltpl = true;
                    double tmp = (roots[i] + roots[j]) / 2;
                    roots[0] = roots[k];
                    roots[1] = roots[2] = tmp;
                }
    }
};

void gcm::ThirdDegreePolynomial::getRoots(double *place)
{
	for (int i = 0; i < 3; i++)
		place[i] = roots[i];
};

bool gcm::ThirdDegreePolynomial::isMultiple()
{
    return isMltpl;
};

gcm::ThirdDegreePolynomial::~ThirdDegreePolynomial() {};

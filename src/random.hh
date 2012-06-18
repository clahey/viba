#ifndef RANDOM_HH_
#define RANDOM_HH_

#include <cmath>

double GetNormalDistribution(double sigma,
			     double minimum = -HUGE_VAL,
			     double maximum = HUGE_VAL);

#endif /* RANDOM_HH_ */

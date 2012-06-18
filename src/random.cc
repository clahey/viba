
#include "random.hh"

#include <boost/random.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>

typedef boost::variate_generator<boost::mt19937&, 
				 boost::normal_distribution<> > Generator;
static boost::mt19937 mSource;
static Generator* mRNG;

double
GetNormalDistribution(double sigma, double minimum, double maximum)
{
  if (mRNG == NULL) {
    mRNG = new Generator(mSource, boost::normal_distribution<>());
  }
  double value = (*mRNG)() * sigma;
  value = std::max(std::min(value, maximum), minimum);
  return value;
}

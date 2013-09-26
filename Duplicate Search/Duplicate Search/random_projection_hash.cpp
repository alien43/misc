#include "random_projection_hash.h"

#include <boost/bind.hpp>
#include <boost/random.hpp>

#include <algorithm>

using std::vector;

RPHash::RPHash(size_t maxId, size_t numberOfVectors)
: maxId_(maxId), nVectors_(numberOfVectors)
{
	buildRandomVectors();
}

bool scalarProduct(const RPHash::DocumentVector& dv, const vector<double>& v)
{
	double res = 0.0;
	for (RPHash::DocumentVector::const_iterator it = dv.begin();
		it != dv.end(); ++it)
	{
		res += v[it->first] * it->second;	
	}
	return res > 0.0;
}

RPHash::ResultType RPHash::get(const RPHash::DocumentVector& dv) const
{
	ResultType result(nVectors_);
	for (size_t i = 0; i < result.size(); ++i) {
		result[i] = scalarProduct(dv, v_->at(i));
	}
	return result;
}

void RPHash::buildRandomVectors()
{  
	boost::mt19937 rng;
	boost::uniform_on_sphere<> sphere(maxId_);
	boost::variate_generator<boost::mt19937&, boost::uniform_on_sphere<> >
		gen(rng, sphere);

	v_.reset(new std::vector< std::vector<double> > (nVectors_));
	std::generate(v_->begin(), v_->end(), gen);
}

double similarity(const RPHash::ResultType& lhs, const RPHash::ResultType& rhs)
{
	if (lhs.size() != rhs.size()) {
		THROW_EX("Results have different sizes");
	}
	if (lhs.size() == 0) {
		THROW_EX("Results have zero sizes");
	}

	size_t distance = 0;
	for (size_t i = 0; i < lhs.size(); ++i) {
		distance += ((lhs[i] == rhs[i]) ? 1 : 0);
	}

	return 1.0 / lhs.size() * distance;
}
#ifndef RANDOM_PROJECTION_HASH_H_INCLUDED
#define RANDOM_PROJECTION_HASH_H_INCLUDED

#include "common.h"

#include <boost/scoped_ptr.hpp>
#include <boost/dynamic_bitset.hpp>

#include <vector>
#include <map>

const size_t NUMBER_OF_VECTORS = 100;

class RPHash {
public:
	typedef std::map<Id, uint> DocumentVector;
	typedef boost::dynamic_bitset<> ResultType;

	RPHash(size_t maxId, size_t numberOfVectors);

	/*
		map's ids must be in [0; maxId) interval;
		result bool vector has size equal to hashDimension
	*/
	ResultType get(const DocumentVector&) const;

private:
	void buildRandomVectors();

	size_t maxId_;
	size_t nVectors_;
	boost::scoped_ptr< std::vector< std::vector<double> > > v_;
};

double similarity(const RPHash::ResultType&, const RPHash::ResultType&);

#endif //RANDOM_PROJECTION_HASH_H_INCLUDED

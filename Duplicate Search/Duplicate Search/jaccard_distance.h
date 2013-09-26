#ifndef JACCARD_DISTANCE_H_INCLUDED
#define JACCARD_DISTANCE_H_INCLUDED

#include "common.h"
#include "counter_iterator.h"

#include <vector>

#include <iostream>
#include <fstream>

// the vectors must be sorted
template<class T>
double jaccardDistance(const std::vector<T>& lhs, const std::vector<T>& rhs)
{
	if (lhs.empty() || rhs.empty()) {
		THROW_EX("Both vectors are empty");
	}
	CounterIterator counter;
	std::set_intersection(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), 
		counter);
	const size_t intersectionSize = counter.count();
	const size_t unionSize = lhs.size() + rhs.size() - intersectionSize;

	const double res = 1.0 * intersectionSize / unionSize;
	return res;
}

#endif //JACCARD_DISTANCE_H_INCLUDED

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>

#include <boost/assign/std/vector.hpp>
using boost::assign::operator +=;

size_t simpleFindNumberOfInversions(const std::vector<int>& vector)
{
    size_t nInversions = 0;
    for (size_t i = 0; i < vector.size(); ++i) {
        for (size_t j = i + 1; j < vector.size(); ++j) {
            if (vector[i] > vector[j]) {
                ++nInversions;
            }
        }
    }
    return nInversions;
}


template<class Iterator, class OutputIterator>
size_t inversionsCountingMerge(Iterator firstBegin, Iterator firstEnd,
    Iterator secondBegin, Iterator secondEnd, OutputIterator itOutput)
{
    size_t nInversions = 0;
    Iterator itFirst = firstBegin;
    Iterator itSecond = secondBegin;
    while (itFirst != firstEnd && itSecond != secondEnd) {
        if (*itFirst <= *itSecond) {
            *itOutput++ = *itFirst++;
        } else {
            *itOutput++ = *itSecond++;
            nInversions += (firstEnd - itFirst);
        }
    }
    nInversions += std::distance(itFirst, firstEnd) * std::distance(itSecond, secondEnd);
    std::copy(itFirst, firstEnd, itOutput);
    std::copy(itSecond, secondEnd, itOutput);
    return nInversions;
}

template<class Iterator>
size_t countInversions(Iterator begin, Iterator end)
{
    const std::ptrdiff_t size = std::distance(begin, end);
    if (size <= 1) {
        return 0;
    }
    const Iterator median = begin + size / 2;
    size_t result = 0;
    result += countInversions(begin, median);
    result += countInversions(median, end);
    std::vector< typename std::iterator_traits<Iterator>::value_type > resultVector(size);
    result += inversionsCountingMerge(begin, median, median, end, resultVector.begin());
    std::copy(resultVector.begin(), resultVector.end(), begin);
    return result;
}

size_t findNumberOfInversions(std::vector<int> vector)
{
    return countInversions(vector.begin(), vector.end());
}

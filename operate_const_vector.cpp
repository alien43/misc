#include <boost/range/iterator_range.hpp>

#include <boost/assign/std/vector.hpp>
using boost::assign::operator+=;

#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>


template<class T>
boost::iterator_range<const T*> makePointerRange(const std::vector<const T>& vector)
{
    return boost::make_iterator_range(&(vector[0]), &(vector[0]) + vector.size());
}

template<class T>
boost::iterator_range<const T*> makePointerRange(const std::vector<T>& vector)
{
    return boost::make_iterator_range(&(vector[0]), &(vector[0]) + vector.size());
}

typedef boost::iterator_range<const int*> ConcreteConstRange;

void operateConstRange(const ConcreteConstRange& range)
{
    std::copy(range.begin(), range.end(),
        std::ostream_iterator<const int>(std::cout, " "));
    std::cout << std::endl;
}

namespace {

int main_()
{
    std::vector<const int> vectorOfConst;
    vectorOfConst += 1, 5, 4, 3;
    operateConstRange(makePointerRange(vectorOfConst));

    std::vector<int> vector;
    vector += 1, 5, 4, 3;
    operateConstRange(makePointerRange(vector));

    return 0;
}

}
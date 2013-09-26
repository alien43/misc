#include <boost/iterator/transform_iterator.hpp>
#include <boost/range/iterator_range.hpp>

#include <functional>

template<class Pair, class First>
struct select1st : std::unary_function<Pair, First>
{
    First operator() (Pair pair) const
    {
        return pair.first;
    }
};

template<class Pair, class Second>
struct select2nd : std::unary_function<Pair, Second>
{
    Second operator() (Pair pair) const
    {
        return pair.second;
    }
};

template<class Container>
struct Types {
    typedef typename Container::value_type value_type;

    
    typedef typename value_type::first_type first_type;
    typedef typename value_type::second_type second_type;

    
    typedef select1st<const value_type&, const first_type&> first_const_selector;
    typedef select1st<value_type&, first_type&> first_selector;
    
    typedef select2nd<const value_type&, const second_type&> second_const_selector;
    typedef select2nd<value_type&, second_type&> second_selector;


    typedef boost::transform_iterator<
        first_selector, typename Container::iterator
    > first_iterator;
    typedef boost::transform_iterator<
        first_const_selector, typename Container::const_iterator
    > first_const_iterator;


    typedef boost::transform_iterator<
        second_selector, typename Container::iterator
    > second_iterator;
    typedef boost::transform_iterator<
        second_const_selector, typename Container::const_iterator
    > second_const_iterator;


    typedef boost::iterator_range<first_iterator> first_range;
    typedef boost::iterator_range<first_const_iterator> first_const_range;

    typedef boost::iterator_range<second_iterator> second_range;
    typedef boost::iterator_range<second_const_iterator> second_const_range;
};

template<class Container>
typename Types<Container>::first_const_range keys(const Container& container)
{
    typedef Types<Container> Types;
    return typename Types::first_const_range(
        typename Types::first_const_iterator(
            container.begin(), typename Types::first_const_selector()
            ),
        typename Types::first_const_iterator(
            container.end(), typename Types::first_const_selector()
            ));
}

template<class Container>
typename Types<Container>::first_range keys(Container& container)
{
    typedef Types<Container> Types;
    return typename Types::first_range(
        typename Types::first_iterator(
            container.begin(), typename Types::first_selector()
            ),
        typename Types::first_iterator(
            container.end(), typename Types::first_selector()
            ));
}

template<class Container>
typename Types<Container>::second_const_range values(const Container& container)
{
    typedef Types<Container> Types;
    return typename Types::second_const_range(
        typename Types::second_const_iterator(
            container.begin(), typename Types::second_const_selector()
            ),
        typename Types::second_const_iterator(
            container.end(), typename Types::second_const_selector()
            ));
}

template<class Container>
typename Types<Container>::second_range values(Container& container)
{
    typedef Types<Container> Types;
    return typename Types::second_range(
        typename Types::second_iterator(
            container.begin(), typename Types::second_selector()
            ),
        typename Types::second_iterator(
            container.end(), typename Types::second_selector()
            ));
}



#include <boost/foreach.hpp>
#include <iostream>
#include <map>
#include <algorithm>

namespace {

int main_()
{
    typedef std::map<int, double> Map;
    Map map;
    map[1] = 5.0; map[3] = 3.0;
    BOOST_FOREACH(int x, keys(map)) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    BOOST_FOREACH(double& x, values(map)) {
        x += 1.0;
    }
    BOOST_FOREACH(double x, values(const_cast<const Map&>(map))) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    std::cout << *std::max_element(values(map).begin(), values(map).end()) << std::endl;

    return 0;
}

}
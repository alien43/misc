#include <string>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/fusion/include/for_each.hpp>


struct func {
    template< typename T, typename S >
    void operator() ( T const& t, S const& s ) const
    {
        std::cout << "[ (" << typeid(t).name() << ") " << t
            << ", (" << typeid(s).name() << ") " << s << " ]\n";
    }
};

struct tuple_for_each
{
    template< typename T, typename S, typename F >
    void operator()( T const& t, S const& s, F f ) const
    {
        boost::fusion::for_each( t, boost::bind<void>(f, s, _1) );
    }
};
 
template< typename T1, typename T2, typename F >
void cartesian_product( T1 const& t1, T2 const& t2, F f )
{
    boost::fusion::for_each( t1, boost::bind<void>( tuple_for_each(), t2, _1, f ) );
}

int mainy()
{
    boost::tuple<int, double, long, std::string> t1(1, 2.0, 3L, "s");
    boost::tuple<int, float, short> t2(11, 12.0, 13);
    cartesian_product( t1, t2, func());


	return 0;
}




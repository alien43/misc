#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

#include <boost/assign/std/vector.hpp>

using boost::assign::operator +=;

struct Foo {
    static int a()
    {
        return 5;
    }

    int b() const
    {
        return 6;
    }
};


namespace {

int main_()
{
    typedef int (*SF)();
    SF sf = &Foo::a;
    std::cout << (*sf)() << std::endl;

    typedef int (Foo::*F)() const;
    F f = &Foo::b;

    std::cout << (Foo().*f)() << std::endl;

    return 0;
}

}

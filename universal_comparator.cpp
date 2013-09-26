#include <boost/utility/result_of.hpp>

#include <functional>

template<class Class>
class UniversalComparator {
public:
    UniversalComparator(const Class& first, const Class& second)
        : result_(0), first_(&first), second_(&second)
    {}

    template<class T>
    UniversalComparator& operator() (T t)
    {
        return (*this)(t, std::less<typename boost::result_of<T()>::type>());
    }

    template<class T, class Comparator>
    UniversalComparator& operator() (T t, Comparator comparator)
    {
        compare<typename boost::result_of<T()>::type, Comparator>(
            t, comparator);
        return *this;
    }

    //! Returns -1, 0 or 1 if first is less than, equals or greater than second
    //! respectively.
    int result() const
    {
        return result_;
    }

    bool less() const
    {
        return result_ == -1;
    }

    bool equal() const
    {
        return result_ == 0;
    }

    bool greater() const
    {
        return result_ == 1;
    }

    bool lessEqual() const
    {
        return result <= 0;
    }

    bool greaterEqual() const
    {
        return result >= 0;
    }

private:
    template<class T>
    struct Method {
        typedef T (Class::*type)() const;
    };

    template<class T, class Comparator>
    void compare(typename Method<T>::type method, Comparator comparator)
    {
        if (result_ != 0) {
            return;
        }

        const T& firstValue = (first_->*method)();
        const T& secondValue = (second_->*method)();
        if (comparator(firstValue, secondValue)) {
            result_ = -1;
        } else if (comparator(secondValue, firstValue)) {
            result_ = 1;
        }
    }

    int result_;
    const Class* first_;
    const Class* second_;
};

template<class T>
UniversalComparator<T> compareUniversally(const T& first, const T& second)
{
    return UniversalComparator<T>(first, second);
}


//! Example

class Foo {
public:
    Foo(int a, double b)
        : a_(a), b_(b)
    {}

    const int& a() const
    {
        return a_;
    }

    double b() const
    {
        return b_;
    }

private:
    int a_;
    double b_;
};

#include <iostream>

namespace {

int main_()
{
    Foo x(2, 3.0);
    Foo y(1, 4.0);

    std::cout
        << compareUniversally(x, y)(&Foo::a)(&Foo::b).result() << "\n"
        << compareUniversally(x, y)(&Foo::b)(&Foo::a).result() << "\n"
        << compareUniversally(x, y)(&Foo::a, std::greater<int>())(&Foo::b).result() << "\n";
    x = Foo(1, 5.0);
    std::cout << compareUniversally(x, y)(&Foo::a).result() << "\n";

    std::cout << std::endl;

    return 0;
}

}

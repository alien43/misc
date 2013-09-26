#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/iterator/indirect_iterator.hpp>

#include <algorithm>
#include <vector>
#include <memory>
#include <string>
#include <iostream>

namespace {

template<class T>
class Wor {
public:
    typedef const T* Pointer;
    typedef const T& Reference;

    Wor()
        : objectsPtr_(new boost::ptr_vector<T>)
    {}

    Wor(Wor& another)
    {
        objectsPtr_ = another.objectsPtr_;
        std::swap(pointers_, another.pointers_);
    }

    Wor& operator= (Wor& another)
    {
        objectsPtr_ = another.objectsPtr_;
        std::swap(pointers_, another.pointers_);
    }



    void add(Pointer pointer)
    {
        pointers_.push_back(pointer);
    }

    void add(Reference reference)
    {
        objectsPtr_->push_back(std::auto_ptr<T>(new T(reference)));
        add(&(objectsPtr_->back()));
    }

    template<class S>
    bool contains(const S& object) const
    {
        return std::find(
            boost::make_indirect_iterator(pointers_.begin()),
            boost::make_indirect_iterator(pointers_.end()),
            object) != boost::make_indirect_iterator(pointers_.end());
    }

private:
    std::auto_ptr< boost::ptr_vector<T> > objectsPtr_;
    std::vector<Pointer> pointers_;
};

template<class S, class T>
bool operator == (const S& object, const Wor<T>& wor)
{
    return wor.contains(object);
}

template<class S, class T>
bool operator == (const Wor<T>& wor, const S& s)
{
    return s == wor;
}

template<class S, class T>
Wor<T> operator || (Wor<T> wor, const S& s)
{
    wor.add(s);
    return wor;
}

template<class S, class T>
Wor<T> operator || (const S& s, Wor<T> wor)
{
    return wor || s;
}


template<class T, class S>
Wor<T> wor(const S& object, T* = 0)
{
    Wor<T> w;
    w.add(object);
    return w;
}

template<class T>
Wor<T> wor(const T& object)
{
    return wor<T, T>(object, static_cast<T*>(0));
}

template<class T>
Wor<T> wor(const T* pointer)
{
    return wor(object, static_cast<T*>(0));
}

Wor<std::string> wor(const char* cStr)
{
    return wor(std::string(cStr));
}


int main2()
{
    std::string name = "vasya";
    std::cout << (name == (wor("petya") || "masha")) << std::endl;
    std::cout << (name == (wor("petya") || "masha" || "vasya")) << std::endl;

    int a = 5;
    int b = 4;
    std::cout << (a == (wor(3) || &b)) << std::endl;

    return 0;
}

}

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

#include <string>
#include <iostream>
#include <sstream>


class S {
public:
    S()
    {}

    std::ostream& stream() {
        return oss_;
    }

    std::string str() const {
        return oss_.str();
    }

    operator std::string() const {
        return str();
    }

    operator const char*() const {
        return str().c_str();
    }

private:
    std::ostringstream oss_;
};

template<class T>
S& operator << (S& s, const T& t) {
    s.stream() << t;
    return s;
}

std::ostream& operator << (std::ostream& os, const S& s) {
    os << s.str();
    return os;
}

namespace {

int main() {
    for (size_t i = 0; i < 20; ++i) {
        const std::string oldFileName = "myfile_" + boost::lexical_cast<std::string>(i) + ".data";
        const std::string newFileName = (S() << "myfile_" << i << ".data");
        const std::string newerFileName = boost::str(boost::format("myfile_%1%.data") % i);

        std::cout << oldFileName << " vs " << newFileName << " vs " << newerFileName << "\n";
    }

    return 0;
}

}
#include <iostream>

template<class T>
T getBit(T number, size_t iBit)
{
    return (number >> iBit) & 1;
}

template<class T>
size_t numberOfBits(T = 0)
{
    return sizeof(T) * 8;
}

template<class T>
T pow2(size_t exponent)
{
    return static_cast<T>(1) << exponent;
}

template<class T>
void setBit(T* number, size_t iBit, T value)
{
    const T powerOfTwo = pow2<T>(iBit);
    if (value == 0) {
        (*number) &= ~powerOfTwo;
    } else {
        (*number) |= powerOfTwo;
    }
}

template<class T>
T reverseBits(T number)
{
    const size_t nBits = numberOfBits(number);
    T reversed;
    for (size_t iBit = 0; iBit < nBits; ++iBit) {
        setBit(&reversed, iBit, getBit(number, nBits - 1 - iBit));
    }
    return reversed;
}

template<class T>
void printBits(T number, std::ostream& os)
{
    const size_t nBits = sizeof(T) * 8;
    for (size_t iBit = 0; iBit < nBits; ++iBit) {
        os << getBit(number, iBit);
    }
}

void testReverse(int number)
{
    const int reversedNumber = reverseBits(number);
    if ((number & reversedNumber) != 0) {
        std::cerr << "Wrong & operation for number " << number;
    }
}

namespace {

int main_()
{
    int a = 1027;
    int b = reverseBits(a);
    printBits(a, std::cout);
    std::cout << "\n";
    printBits(b, std::cout);
    std::cout << "\n";
    std::cout << a << " " << b << std::endl;

    return 0;
}

}

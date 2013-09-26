#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

char digitToChar(int digit)
{
    return '0' + static_cast<char>(digit);
}

int charToDigit(char ch)
{
    return static_cast<int>(ch - '0');
}

#if 0
using namespace std;

char& digitAsChar(string& number, size_t index)
{
    return *(number.rbegin() + index);
}

int digit(const string& number, size_t index)
{
    return charToDigit(*(number.rbegin() + index));
}

string numbersSum(const string& first, const string& second)
{
    string sum(max(first.size(), second.size()) + 1, digitToChar(0));
    for (size_t iSum = 0; iSum < sum.size(); ++iSum) {
        int digitSum = digit(sum, iSum);
        if (iSum < first.size()) {
            digitSum += digit(first, iSum);
        }
        if (iSum < second.size()) {
            digitSum += digit(second, iSum);
        }
        if (digitSum >= 10) {
           digitAsChar(sum, iSum + 1) = digitToChar(1);
           digitSum -= 10;
        }
        digitAsChar(sum, iSum) = digitToChar(digitSum);
    }
    sum.erase(sum.begin(), sum.begin() + std::min(
        sum.length(), sum.find_first_not_of(digitToChar(0))));
    if (sum.empty()) {
        sum.assign(1, digitToChar(0));
    }
    return sum;
}

#else

typedef std::vector<signed short> Long;

Long stringToLong(const std::string& string)
{
    Long result(string.length());
    std::transform(string.rbegin(), string.rend(), result.begin(), &charToDigit);
    return result;
}

void cutLeadingZeros(Long* number)
{
    size_t newSize = number->size();
    while (newSize > 1 && (*number)[newSize - 1] == 0) {
        --newSize;
    }
    number->resize(newSize);
}

std::string longToString(const Long& number)
{
    // cutLeadingZeros(&number);
    std::string result(number.size(), '\0');
    std::transform(number.begin(), number.end(), result.rbegin(), &digitToChar);
    return result;
}

const int BASE = 10;

Long sum(const Long& first, const Long& second)
{
    Long result(std::max(first.size(), second.size()) + 1, 0);
    for (size_t iSum = 0; iSum + 1 < result.size(); ++iSum) {
        int digitsSum = result[iSum];
        if (iSum < first.size()) {
            digitsSum += first[iSum];
        }
        if (iSum < second.size()) {
            digitsSum += second[iSum];
        }
        if (digitsSum >= BASE) {
            result[iSum + 1] = 1;
            result[iSum] = digitsSum - BASE;
        } else {
            result[iSum] = digitsSum;
        }
    }
    cutLeadingZeros(&result);
    return result;
}

Long multiply(const Long& first, const Long& second)
{
    Long result(first.size() + second.size(), 0);
    for (size_t iFirst = 0; iFirst < first.size(); ++iFirst) {
        for (size_t iSecond = 0; iSecond < second.size(); ++iSecond) {
            const size_t iResult = iFirst + iSecond;
            result[iResult] += first[iFirst] * second[iSecond];
            result[iResult + 1] += result[iResult] / BASE;
            result[iResult] %= BASE;
        }
    }
    for (size_t iResult = 0; iResult + 1 < result.size(); ++iResult) {
        result[iResult + 1] += result[iResult] / BASE;
        result[iResult] %= BASE;
    }
    cutLeadingZeros(&result);
    return result;
}

template<class BinaryOperation>
std::string applyBinaryOperation(const std::string& first, const std::string& second,
    BinaryOperation binaryOperation)
{
    return longToString(binaryOperation(stringToLong(first), stringToLong(second)));
}

std::string numbersSum(const std::string& first, const std::string& second)
{
    return applyBinaryOperation(first, second, &sum);
}

std::string numbersMultiply(const std::string& first, const std::string& second)
{
    return applyBinaryOperation(first, second, &multiply);
}


#endif

template<class BinaryOperation>
void testNumbersOperation(const std::string& first, const std::string& second,
    const std::string& correctResult, BinaryOperation binaryOperation)
{
    const std::string result = binaryOperation(first, second);
    if (result != correctResult) {
        std::cerr << "Test failed: sum of " << first << " and " << second
            << " equals " << result << ", correct is " << correctResult << std::endl;
        throw std::logic_error("Test failed");
    }
}

void testNumbersSum(const std::string& first, const std::string& second,
    const std::string& correctSum)
{
    testNumbersOperation(first, second, correctSum, &numbersSum);
}

void testNumbersMultiply(const std::string& first, const std::string& second,
    const std::string& correctProduct)
{
    testNumbersOperation(first, second, correctProduct, &numbersMultiply);
}

void runTests()
{
    testNumbersSum("00", "0", "0");
    testNumbersSum("1", "0", "1");
    testNumbersSum("10", "0", "10");
    testNumbersSum("5", "04", "9");
    testNumbersSum("05", "5", "10");
    testNumbersSum("9", "1", "10");
    testNumbersSum("99", "001", "100");
    testNumbersSum("1543", "57", "1600");
    testNumbersSum("104", "87", "191");
    testNumbersSum("000023", "1001", "1024");
    testNumbersSum("1", "999", "1000");
    testNumbersSum("9999", "1", "10000");
    std::cerr << "Summation tests passed successfully." << std::endl;

    testNumbersMultiply("1", "0", "0");
    testNumbersMultiply("1543", "0", "0");
    testNumbersMultiply("0", "43", "0");
    testNumbersMultiply("1", "1", "1");
    testNumbersMultiply("2", "2", "4");
    testNumbersMultiply("1543", "100", "154300");
    testNumbersMultiply("77", "13", "1001");
    testNumbersMultiply("25", "25", "625");
    testNumbersMultiply("100", "010", "1000");
    std::cerr << "Multiplication tests passed successfully." << std::endl;
}

namespace {

int main_()
{
    runTests();
    return 0;

    std::string first;
    std::string second;
    std::cin >> first >> second;
    const std::string sum = numbersSum(first, second);
    std::cout << sum << std::endl;

    return 0;
}

}
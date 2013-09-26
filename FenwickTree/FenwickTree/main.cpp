//! Author: Andrey Petrov
//! Implementation of a Fenwick tree with tests


//! This macros allows some methods to be written very briefly
#define USE_SIGNED_INDEX_TYPE

#include <vector>
#include <numeric>
#include <cstdlib>
#include <iostream>

class FenwickTree {
public:
#ifdef USE_SIGNED_INDEX_TYPE
    typedef int Index;
#else
    typedef size_t Index;
#endif

    explicit FenwickTree(Index size)
        : sums_(size, 0)
    {}

    // Takes O(vector size) time to perform
    void initialize(std::vector<int> numbers) {
        // Thanks to someone for this simple idea
        std::partial_sum(numbers.begin(), numbers.end(), numbers.begin());
        sums_.resize(numbers.size());
        for (Index i = 0; i < size(); ++i) {
            sums_[i] = numbers[i];
            Index previousI = previousIndex(i);
            if (previousI > 0) {
                sums_[i] -= numbers[previousI - 1];
            }
        }
    }

    void add(Index index, int value) {
        while (index < size()) {
            sums_[index] += value;
            index |= (index + 1);
        }
    }


    int sum(Index firstIndex, Index lastIndex) const {
#ifdef USE_SIGNED_INDEX_TYPE
        return sum(lastIndex) - sum(firstIndex - 1);
#else
        int result = sum(lastIndex);
        if (firstIndex != 0) {
            result -= sum(firstIndex - 1);
        }
        return result;
#endif
    }

    int at(Index index) const {
        return sum(index, index);
    }

    Index size() const {
        return sums_.size();
    }

private:
    int sum(Index lastIndex) const {
#ifdef USE_SIGNED_INDEX_TYPE
        return lastIndex >= 0
            ? sums_.at(lastIndex) + sum(previousIndex(lastIndex) - 1)
            : 0;
#else
        int result = 0;
        do {
            result += sums_.at(lastIndex);
            lastIndex = previousIndex(lastIndex);
            if (lastIndex != 0) {
                // here last index cannot be 1, so it cannot become 0 after
                // decreasing it by 1
                --lastIndex;                
            }
        } while (lastIndex != 0);
        return result;
#endif
    }

    // Given i, returns i' equal to i in binary notation
    // with all trailing one bits replaced by zeros
    static Index previousIndex(Index index) {
        return index & (index + 1);
    }

    std::vector<int> sums_;
};

class TrivialSummator {
public:
    explicit TrivialSummator(size_t size)
        : numbers_(size, 0)
    {}

    void initialize(const std::vector<int>& numbers) {
        numbers_ = numbers;
    }

    void add(size_t index, int value) {
        numbers_.at(index) += value;
    }

    int sum(size_t firstIndex, size_t lastIndex) const {
        return std::accumulate(
            numbers_.begin() + firstIndex,
            numbers_.begin() + lastIndex + 1,
            0);
    }

private:
    std::vector<int> numbers_;
};

// Returns random number in the [lower; upper) interval
int random(int lowerBound, int upperBound) {
    return lowerBound + std::rand() % (upperBound - lowerBound);
}

void testSummators(size_t size, size_t nOperations) {
    static const int MAX_NUMBER = 1000;

    FenwickTree tree(size);
    TrivialSummator summator(size);
    if (random(0, 3) == 0) {
        // each third time initialize both summators with random numbers
        std::vector<int> numbers(size);
        for (size_t i = 0; i < numbers.size(); ++i) {
            numbers[i] = random(-MAX_NUMBER, MAX_NUMBER);
        }
        tree.initialize(numbers);
        summator.initialize(numbers);
    }
    for (size_t t = 0; t < nOperations; ++t) {
        if (random(0, 2) == 0) {
            // random add operation
            size_t index = random(0, size);
            int value = random(-MAX_NUMBER, MAX_NUMBER);
            tree.add(index, value);
            summator.add(index, value);
        } else {
            // random sum query operation
            size_t firstIndex = random(0, size);
            size_t lastIndex = random(firstIndex, size);
            int treeSum = tree.sum(firstIndex, lastIndex);
            int trivialSum = summator.sum(firstIndex, lastIndex);

            if (treeSum != trivialSum) {
                std::cerr << "wrong\n";
            }
        }
    }
}

int main() {
    // perform all tests
    std::srand(1543);
    for (size_t size = 1; size < 100; ++size) {
        testSummators(size, random(1, 50));
        testSummators(random(100, 1000), random(100, 200));
    }

    return 0;
}

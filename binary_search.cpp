#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>

#include <boost/assign.hpp>

using boost::assign::operator+=;

size_t firstGreater(const std::vector<int>& vector, int value) {
	size_t l = 0;
	size_t r = vector.size();
	while (l < r) {
		size_t m = (l + r) / 2;
		if (vector[m] <= value) {
			l = m + 1;
		} else {
			r = m;
		}
	}
	return l;
}

size_t firstNotLess(const std::vector<int>& vector, int value) {
	size_t l = 0;
	size_t r = vector.size();
	while (l < r) {
		size_t m = (l + r) / 2;
		if (vector[m] < value) {
			l = m + 1;
		} else {
			r = m;
		}
	}
	return l;
}

size_t firstEqualTo(const std::vector<int>& vector, int value) {
	size_t fnl = firstNotLess(vector, value);
	if (fnl < vector.size() && vector[fnl] == value) {
		return fnl;
	} else {
		return vector.size();
	}
}

size_t lastEqualTo(const std::vector<int>& vector, int value) {
	size_t fg = firstGreater(vector, value);
	if (fg == 0) {
		return vector.size();
	}
	--fg;
	if (vector[fg] == value) {
		return fg;
	} else {
		return vector.size();
	}
}


int main() {
	std::vector<int> a;
	a += 1, 1, 2, 3, 5, 5, 7, 8, 10;
	
	std::copy(a.begin(), a.end(), std::ostream_iterator<int>(std::cout, " "));

	for (int i = -1; i < a.back(); ++i) {
		std::cerr << "\n"
			<< "First greater than " << i << " is " << firstGreater(a, i) << "\n"
			<< "First not less than " << i << " is " << firstNotLess(a, i) << "\n"
			<< "First equal to " << i << " is " << firstEqualTo(a, i) << "\n"
			<< "Last equal to " << i << " is " << lastEqualTo(a, i) << "\n"
			;
	}

	return 0;
}
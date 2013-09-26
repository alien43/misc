#ifndef LCS_H_INCLUDED
#define LCS_H_INCLUDED

#include "common.h"

#include <vector>


template<class RandomInputIterator1, class RandomInputIterator2>
size_t longestCommonSubstring(RandomInputIterator1 first1, RandomInputIterator1 last1,
	RandomInputIterator2 first2, RandomInputIterator2 last2)
{
	RandomInputIterator1 it1(first1);
	std::vector<size_t> curr(last2 - first2);
	std::vector<size_t> prev(last2 - first2);
	size_t maxSubstr = 0;
	
	while (it1 != last1)
    {
		const size_t off1 = it1 - first1;
		RandomInputIterator2 it2(first2);	
		while (it2 != last2) {
			const size_t off2 = it2 - first2;
            if (*it1 != *it2) {
				curr[off2] = 0;
            } else {
                if (off1 == 0 || off2 == 0) {
                    curr[off2] = 1;
                } else {
					curr[off2] = 1 + prev[off2 - 1];
                }
                if (maxSubstr < curr[off2]) {
					maxSubstr = curr[off2];
                }
            }
			++it2;
        }
		curr.swap(prev);
		++it1;
    }
    return maxSubstr;
}

#endif //LCS_H_INCLUDED

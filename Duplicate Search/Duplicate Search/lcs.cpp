#include "lcs.h"

#include <vector>

using std::string;
using std::vector; 

size_t longestCommonSubstring(const string& str1, const string& str2)
{
     if (str1.empty() || str2.empty())
     {
		return 0;
     }
 
     vector<size_t> curr(str2.size());
     vector<size_t> prev(str2.size());
     size_t maxSubstr = 0;
     for (size_t i = 0; i < str1.size(); ++i)
     {
         for (size_t j = 0; j < str2.size(); ++j)
         {
             if (str1[i] != str2[j]) {
			     curr[j] = 0;
             } else {
                 if (i == 0 || j == 0) {
                     curr[j] = 1;
                 } else {
					curr[j] = 1 + prev[j-1];
                 }
                 if (maxSubstr < curr[j]) {
					maxSubstr = curr[j];
                 }
             }
         }
		 curr.swap(prev);
     }
     return maxSubstr;
}

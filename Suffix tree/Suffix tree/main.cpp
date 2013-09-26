#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include "suffixtree.h"

int getNumberOfSubstringsNaive (const std::string& str) {
	std::vector<std::string> vectorSubstrings;
	vectorSubstrings.clear();
	int length = (int)str.length();
	for (int leftIndex = 0; leftIndex < length; ++leftIndex)
		for (int rightIndex = leftIndex; rightIndex < length; ++rightIndex)
			vectorSubstrings.push_back (str.substr (leftIndex, rightIndex - leftIndex + 1));

	sort (vectorSubstrings.begin(), vectorSubstrings.end());
	std::vector<std::string>::iterator unique_iterator = unique (vectorSubstrings.begin(), vectorSubstrings.end());
	vectorSubstrings.resize (unique_iterator - vectorSubstrings.begin());
	int answer = vectorSubstrings.size();

	return answer;

}

std::string PerformBWTnaive (const std::string& str) {
	const int length = (int) str.length();
	std::string doubledString = str + str;
	std::vector<std::string> allCyclicShifts;
	allCyclicShifts.clear();
	for (int i = 0; i < length; ++i)
		allCyclicShifts.push_back (doubledString.substr (i, length));
	
/*
	for (int i = 0; i < length; ++i) {
		std::cout << "Cyclic ";
		if (i < 10)
			std::cout << "0";
		std::cout << i << ": " << allCyclicShifts[i] << std::endl;
	}
	std::cout << std::endl;
*/

	sort (allCyclicShifts.begin(), allCyclicShifts.end());

/*
	for (int i = 0; i < length; ++i) {
		std::cout << "Cyclic sorted ";
		if (i < 10)
			std::cout << "0";
		std::cout << i << ": " << allCyclicShifts[i] << std::endl;
	}
*/

	std::string answer = "";
	for (int i = 0; i < length; ++i)
		answer += allCyclicShifts[i][length - 1];
	return answer;
}

std::string PerformBWT2 (const std::string& str) {
	const int length = (int)str.length();
	std::vector<std::string> suffixes;
	suffixes.clear();

	for (int i = 0; i < length; ++i) {
		if (i == length - 2) {
			std::string stmp = "";
			stmp += str[i];
			stmp += str.substr (0, i);
			stmp += "$$";
			suffixes.push_back (stmp);
		}
		else
			suffixes.push_back (str.substr (i, length - i + 1));

	}
/*	
	std::cout << "String: " << str << std::endl;
	for (int i = 0; i < length; ++i) {
		std::cout << "Suffix ";
		if (i < 10)
			std::cout << "0";
		std::cout << i << ": " << suffixes[i] << std::endl;
	}
	std::cout << std::endl;
*/

	sort (suffixes.begin(), suffixes.end());

/*
	for (int i = 0; i < length; ++i) {
		std::cout << "Sorted suffix ";
		if (i < 10)
			std::cout << "0";
		int sl = (int)suffixes[i].length();
		std::cout << i << ": " << suffixes[i];
		std::cout << " " << str.substr (0, length - sl);
		std::cout << std::endl;
	}
	std::cout << std::endl;
*/

	std::string answer = "";
	for (int i = 0; i < length; ++i) {
		int suffixLength = (int)suffixes[i].length();
		if (suffixLength == 1) 
			suffixLength = -1;
		else if (suffixLength == length)
			suffixLength = 1;
		else if (suffixLength > length)
			suffixLength = 2;
		
		if (suffixLength > 0)
			answer += str[length - suffixLength - 1];
	}
	return answer;
}

int main() {
	std::string myString;
//	std::cin >> myString;
	myString = "abcbabcb"; //SIX.MIXED.PIXIES.SIFT.SIXTY.PIXIE.DUST.BOXES
	std::string BWTstring = PerformBWTnaive (myString);
	std::cout << BWTstring << std::endl;

//	myString += "$";
//	BWTstring = PerformBWT2 (myString);
//	std::cout << BWTstring << std::endl;


	SuffixTree suffixTree (myString + myString);
	std::cout << suffixTree.getBWTstring() << std::endl;
	return 0;
}

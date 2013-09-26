#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <string>
#include <utility>
#include <iostream>


typedef unsigned int uint;
typedef std::string Word;
typedef std::pair<Word, Word> Pair;

namespace WordsNumber {
	struct Single {};
	struct Pair {};
};

template<class T>
struct Phrase {
};

template<> struct Phrase<WordsNumber::Single> {
	typedef Word type;
};

template<> struct Phrase<WordsNumber::Pair> {
	typedef Pair type;
};

template<class T, class S>
std::ostream& operator << (std::ostream& os, const std::pair<T, S>& pair)
{
	os << pair.first << " " << pair.second;
	return os;
}

template<class T, class S>
std::istream& operator >> (std::istream& is, std::pair<T, S>& pair)
{
	is >> pair.first >> pair.second;
	return is;
}

#endif //COMMON_H_INCLUDED

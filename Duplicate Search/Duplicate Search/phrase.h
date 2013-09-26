#ifndef PHRASE_H_INCLUDED
#define PHRASE_H_INCLUDED

#include "common.h"

#include <boost/array.hpp>

#include <iostream>

template<size_t N>
struct Phrase
{
	typedef boost::array<Word, N> type;
	static const size_t number_of_words = N;
};

template<size_t N>
void readPhrase(std::istream& is, typename Phrase<N>::type* phrase)
{
	for (size_t iWord = 0; iWord < phrase->size(); ++iWord) {
		is >> phrase->at(iWord);
	}
}

typedef Phrase<1>::type PWord;

inline PWord createPWord(const std::string& s)
{
	PWord result;
	result[0] = s;
	return result;
}

#endif //PHRASE_H_INCLUDED

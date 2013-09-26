#ifndef BAD_PHRASES_H_INCLUDED
#define BAD_PHRASES_H_INCLUDED

#include "phrase.h"

#include <fstream>
#include <set>
#include <string>

template<size_t N>
struct BadPhrases {
	typedef typename Phrase<N>::type PhraseType;
	typedef std::set<PhraseType> type;
	
	typedef typename type::const_iterator const_iterator;
	typedef typename type::iterator iterator;
};

template<size_t N>
void readBadPhrases(const std::string& fileName, typename BadPhrases<N>::type* badPhrases)
{
	std::ifstream fin(fileName.c_str());
	if (!fin.is_open()) {
		std::cerr << "Cannot open bad phrases file " << fileName << "\n";
	}
	fin >> std::ws;
	badPhrases->clear();
	while (!fin.eof()) {
		typename BadPhrases<N>::PhraseType phrase;
		readPhrase<N>(fin, &phrase);
		badPhrases->insert(phrase);
		fin	>> std::ws;
	}
	fin.close();
}

#endif //BAD_PHRASES_H_INCLUDED

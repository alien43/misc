#ifndef VOCABULARY_H_INCLUDED
#define VOCABULARY_H_INCLUDED

#include "phrase.h"

#include <map>
#include <string>

template<size_t N>
struct Vocabulary {
	typedef std::map<typename Phrase<N>::type, Id> type;
};

#endif //VOCABULARY_H_INCLUDED

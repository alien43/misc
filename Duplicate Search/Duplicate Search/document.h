#ifndef DOCUMENT_H_INCLUDED
#define DOCUMENT_H_INCLUDED

#include "common.h"

#include <vector>

struct Document {
	typedef Id Element;
	typedef std::vector<Element> WordsVector;
	typedef WordsVector::const_iterator const_iterator;
	typedef WordsVector::iterator iterator;

	std::string title;
	WordsVector words;
};

#endif //DOCUMENT_H_INCLUDED

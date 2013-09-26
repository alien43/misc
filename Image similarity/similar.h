#ifndef SIMILAR_H_INCLUDED
#define SIMILAR_H_INCLUDED

#include "collection.h"
#include "query.h"

#include <string>

void printSimilar(const Query& query, const Collection& collection,
	const std::string& outputFileName);

#endif //SIMILAR_H_INCLUDED

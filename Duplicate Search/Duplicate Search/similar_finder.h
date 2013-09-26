#ifndef SIMILAR_FINDER_H_INCLUDED
#define SIMILAR_FINDER_H_INCLUDED

#include "common.h"

#include <vector>
#include <utility>

class TraitHandler; 

typedef std::vector<size_t> Indexes;

void findSimilar(const TraitHandler&, std::vector<Indexes>* result);

#endif //SIMILAR_FINDER_H_INCLUDED

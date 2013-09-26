#ifndef SHINGLE_H_INCLUDED
#define SHINGLE_H_INCLUDED

#include "document.h"

double resemblance(const Document&, const Document&, size_t shingleSize);

#endif //SHINGLE_H_INCLUDED

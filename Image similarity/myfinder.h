#ifndef MYFINDER_H_INCLUDED
#define MYFINDER_H_INCLUDED

#include "similar_finder.h"

class MyFinder : public SimilarFinder {
public:
	MyFinder()
		: SimilarFinder()
	{}

	double find(const ImageTrait& imgQuery, const ImageTrait& imgCollected) const;

	double threshold() const;
};


#endif //MYFINDER_H_INCLUDED

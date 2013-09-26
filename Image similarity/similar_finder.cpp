#include "similar_finder.h"
#include "common.h"
#include "image_trait.h"

SimilarFinder* SimilarFinder::instance_ = 0;

SimilarFinder* SimilarFinder::instance()
{
	if (instance_ == 0) {
		THROW_EX("SimilarFinder is not instantiated");
	}
	return instance_;
}

void SimilarFinder::setInstance(SimilarFinder* newInstance)
{
	instance_ = newInstance;
}

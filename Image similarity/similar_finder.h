#ifndef SIMILAR_FINDER_H_INCLUDED
#define SIMILAR_FINDER_H_INCLUDED

class ImageTrait;

class SimilarFinder {
public:
	static SimilarFinder* instance();
	static void setInstance(SimilarFinder*);

	//! Returns similarity from 0 to 1 of two images.
	virtual double find(const ImageTrait& imgQuery, const ImageTrait& imgCollected) const = 0;
	//! Returns least value for which we consider images to be similar.
	virtual double threshold() const = 0;

protected:
	SimilarFinder()
	{}

private:
	static SimilarFinder* instance_;
};

#endif //SIMILAR_FINDER_H_INCLUDED

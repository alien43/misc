#include "similar.h"
#include "similar_finder.h"
#include "common.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <utility>

using std::string;
using std::vector;

typedef vector<string> Result;


struct Similarity {
	Similarity(double prob, const ImageTrait* trait)
		: probability(prob), imageTrait(trait)
	{
	}

	double probability;
	const ImageTrait* imageTrait;
};

struct SimilarityComparator {
	bool operator () (const Similarity& lhs, const Similarity& rhs)
	{
		return lhs.probability > rhs.probability;
	}
};

Result getResult(const Query& query,
	const Collection& collection)
{
	const ImageTrait imgQuery(query.path());
	Result result;
	vector<Similarity> similarities;
	similarities.reserve(collection.size());
	for (size_t iImage = 0; iImage < collection.size(); ++iImage) {
		const ImageTrait& imgCollected = collection.traitAt(iImage);
		const double prob = SimilarFinder::instance()->find(imgQuery, imgCollected);
		similarities.push_back(Similarity(prob, &imgCollected));
	}
	std::sort(similarities.begin(), similarities.end(), SimilarityComparator());
	vector<Similarity>::const_iterator it = std::lower_bound(
		similarities.begin(), similarities.end(),
		Similarity(SimilarFinder::instance()->threshold(), 0),
		SimilarityComparator());
	for (size_t iImg = 0; similarities.begin() + iImg < it; ++iImg) {
		result.push_back(similarities.at(iImg).imageTrait->path().name());
	}
	return result;
}

void printSimilar(const Query& query,
	const Collection& collection, const string& outputFileName)
{
	const Result result = getResult(query, collection);
	std::ofstream fout(outputFileName.c_str());
	std::copy(result.begin(), result.end(), 
		std::ostream_iterator<string>(fout, "\n"));
	fout.close();
}

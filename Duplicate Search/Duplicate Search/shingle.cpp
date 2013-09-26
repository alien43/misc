#include "shingle.h"
#include "jaccard_distance.h"

#include <algorithm>
#include <deque>

typedef std::deque<Document::Element> Shingle;

struct ShingleComparator {
	bool operator() (const Shingle& lhs, const Shingle& rhs) const
	{
		if (lhs.size() != rhs.size()) {
			THROW_EX("Shingles to compare have different sizes.");
		}
		return std::lexicographical_compare(lhs.begin(), lhs.end(),
			rhs.begin(), rhs.end());
	}
};

void createShingles(const Document& doc, size_t size, std::vector<Shingle>* shingles)
{
	shingles->clear();
	if (doc.words.size() < size || size == 0) {
		return;
	}
	Document::const_iterator itWord = doc.words.begin() + size;
	Shingle current(doc.words.begin(), itWord);
	while (true) {
		shingles->push_back(current);
		current.pop_front();
		if (itWord == doc.words.end()) {
			break;
		}
		current.push_back(*itWord);
		++itWord;
	} 
	std::sort(shingles->begin(), shingles->end(), ShingleComparator());
}

double resemblance(const Document& d1, const Document& d2, size_t shingleSize)
{
	std::vector<Shingle> s1, s2;
	createShingles(d1, shingleSize, &s1);
	createShingles(d2, shingleSize, &s2);
	const double res = jaccardDistance(s1, s2);
	return res;
}

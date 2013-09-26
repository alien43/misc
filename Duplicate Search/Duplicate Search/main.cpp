#include "file_reader.h"
#include "random_projection_hash.h"
#include "similar_finder.h"
#include "shingle.h"
#include "handlers.h"
#include "lcs.h"

#include <boost/bind.hpp>

#include <iostream>
#include <fstream>
#include <numeric>

using namespace std;

typedef Vocabulary<1>::type Vocab1;
typedef BadPhrases<1>::type Bad1;
typedef Processor<1> Proc1;

void writeAnswer(const string& answerFileName, const string& outputFileName,
	const vector<string>& titles, const SaveHandler& saveHandler,
	const vector<Indexes>& similar)
{
	const double svThreshold = 5.0;
	const double lscThreshold = 0.75;


	std::ofstream fout(outputFileName.c_str());
	std::ofstream fans(answerFileName.c_str());
	if (!fout.is_open() || !fans.is_open()) {
		return;
	}

	size_t iTitle = 0;
	for (size_t iPack = 0; iPack < similar.size(); ++iPack) {
		fout << std::string(40, '=') << "\n";
		fout << "Pack with size = " << similar[iPack].size() << "\n";
		for (size_t iFirst = 0; iFirst < similar[iPack].size(); ++iFirst) {
		for (size_t iSecond = 0; iSecond < iFirst; ++iSecond) {
			const Document& doc1 = saveHandler.find(titles[iTitle + iFirst]);
			const Document& doc2 = saveHandler.find(titles[iTitle + iSecond]);
			std::vector<double> shingleValues;
			double shinglesValue = 0.0;

			for (size_t shingleSize = 1; shingleSize <= 3; ++shingleSize) {
				shingleValues.push_back(resemblance(doc1, doc2, shingleSize));
				shinglesValue += shingleValues.back() * shingleSize;
			}

			if (shinglesValue > svThreshold) {
				fout << "Comparing " << doc1.title << " and " << doc2.title << "\n"
					 << "shingles value = " << shinglesValue << "\n";
				fout << "one by one: ";
				std::copy(shingleValues.begin(), shingleValues.end(),
					std::ostream_iterator<double>(fout, " "));
				fout << "\n";
				const size_t lsc = longestCommonSubstring(
					doc1.words.begin(), doc1.words.end(),
					doc2.words.begin(), doc2.words.end());
				const double lscRate = 1.0 * lsc / std::min(doc1.words.size(), doc2.words.size());
				fout << "LSC rate = " << lscRate << "\n";

				if (lscRate > lscThreshold) {
					fout << "LSC confirms\n";
					fans << doc1.title << " AND " << doc2.title << endl;
				} else {
					fout << "LSC rejects\n";
				}
				fout << "\n";
			}
		}	
		}
		iTitle += similar[iPack].size();
	}
	fans.close();
	fout.close();
}

int main()
{
	const string path = "C:\\YS\\text\\";
	const string suffix = "_with_numbers";
	const string wikiFileName(path + "wiki" + suffix + ".txt");
	const string badWordsFileName(path + "bad_words.txt");
	const string outputFileName(path + "duplicates" + suffix + ".out");
	const string answerFileName(path + "duplicates" + suffix + ".ans");

	Bad1 badWords;
	std::cout << "Reading bad words..." << std::endl;
	readBadPhrases<1>(badWordsFileName, &badWords);
	badWords.insert(createPWord(""));
	badWords.insert(createPWord("."));

	auto_ptr<Vocab1> vocabulary(new Vocab1);
	Proc1 processor(badWords, vocabulary.get());
	std::cout << "Filling vocabulary..." << std::endl;
	processFile<1>(wikiFileName, &processor);

	RPHash hasher(vocabulary->size(), 48);
	TraitHandler traitHandler(&hasher);
	std::cout << "Handling hashes..." << std::endl;
	processFile<1>(wikiFileName, &processor, &traitHandler);

	std::vector<Indexes> similar;
	std::cout << "Finding similar..." << std::endl;
	findSimilar(traitHandler, &similar);

	std::vector<std::string> titles;
	for (std::vector<Indexes>::const_iterator it = similar.begin();
		it != similar.end(); ++it)
	{
		for (Indexes::const_iterator itIndex = it->begin();
			itIndex != it->end(); ++itIndex)
		{
			titles.push_back(traitHandler.traitAt(*itIndex).title);
		}
	}

	SaveHandler saveHandler(titles);
	std::cout << "Loading similar documents..." << std::endl;
	processFile<1>(wikiFileName, &processor, &saveHandler);
	std::cout << "Calculating output...\n";

	writeAnswer(answerFileName, outputFileName, titles, saveHandler, similar);

	return 0;
}

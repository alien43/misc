#include "common.h"

#include <cstdio>
#include <memory>
#include <map>
#include <set>
#include <vector>
#include <cassert>
#include <algorithm>
#include <fstream>
#include <iterator>

using std::string;
using std::set;

const size_t MAX_LINE_SIZE = 10000000;
const uint REMOVER_THRESHOLD = 50;

namespace WN = WordsNumber;


template<class T>
struct DeleteThreshold;

template<>
struct DeleteThreshold<WN::Single> {
	static const uint value = 4;
};

template<>
struct DeleteThreshold<WN::Pair> {
	static const uint value = 2;
};

template<class T>
class PhraseCounter {
public:
	typedef typename Phrase<T>::type PhraseType;
	typedef std::map<PhraseType, uint> FrequencyMap;
	typedef typename FrequencyMap::value_type value_type;

	PhraseCounter()
		: freqs_(new FrequencyMap)
	{}

	void addPhrase(const PhraseType& phrase)
	{
		// increase counter, inserting new phrase at first, if needed
		++((freqs_->insert(std::make_pair(phrase, 0))).first->second);

		++numberOfPhrases_;
		if (numberOfPhrases_% CLEANUP_PERIOD == 0) {
			doCleanup();
		}
	}

	const FrequencyMap& getFrequencies() const
	{
		return *freqs_;
	}

private:
	static const unsigned int CLEANUP_PERIOD = 1000000;

	void doCleanup()
	{
		for (typename FrequencyMap::iterator it = freqs_->begin();
			it != freqs_->end();)
		{
			if (it->second < typename DeleteThreshold<T>::value) {
				it = freqs_->erase(it);
			} else {
				++it;
			}
		}
	}

	size_t numberOfPhrases_;
	std::auto_ptr<FrequencyMap> freqs_;
};

class Processor {
public:
	typedef PhraseCounter<WN::Single> WordCounter;
	typedef PhraseCounter<WN::Pair> PairCounter;

	// Uses reference to the bad phrases sets
	Processor(
		const set<Word>& badWords,
		const set<Pair>& badPairs)
		: nTexts_(0), nWords_(0), badWords_(badWords), badPairs_(badPairs)
	{}

	void process(const string& text)
	{
		Phrase<WN::Pair>::type context;
		for (size_t iChar = 0; iChar < text.length(); ++iChar) {
			const char c = text[iChar];
			if (c == ' ' || iChar + 1 == text.length()) {
				if (isGood(context.first)) {
					++nWords_;
					wordCounter_.addPhrase(context.first);
				}
				if (isGood(context)) {
					pairCounter_.addPhrase(context);
				}
				context.first = context.second;
				context.second = "";
			} else {
				context.second += c;
			}
		}
		if (isGood(context.second)) {
			++nWords_;
			wordCounter_.addPhrase(context.second);
		}
		++nTexts_;
	}

	const WordCounter& getCounter(WN::Single) const
	{
		return wordCounter_;
	}

	const PairCounter& getCounter(WN::Pair) const
	{
		return pairCounter_;
	}

	size_t totalWordsNumber() const
	{
		return nWords_;
	}

private:
	bool isGood(const Word& word) const
	{
		return (badWords_.count(word) == 0);
	}

	bool isGood(const Pair& pair) const
	{
		return (pair.first != pair.second
			&& isGood(pair.first) && isGood(pair.second)
			&& badPairs_.count(pair) == 0);
	}


	size_t nTexts_;
	size_t nWords_;
	const set<Word>& badWords_;
	const set<Pair>& badPairs_;
	WordCounter wordCounter_;
	PairCounter pairCounter_;
};


template<class T>
struct Comparator {
	bool operator() (
		const std::pair<T, unsigned int>& lhs, 
		const std::pair<T, unsigned int>& rhs)
	{
		return lhs.second > rhs.second;
	}
};


template<class T>
class Remover {
public:
	Remover(uint threshold)
		: threshold_(threshold)
	{}

	bool operator() (const typename PhraseCounter<T>::value_type& value) const
	{
		return value.second < threshold_;
	}

private:
	uint threshold_;
};

template<class T>
class Writer {
public:
	Writer(std::ostream& os, const Processor& processor)
		: os_(os), processor_(processor)
	{}

	void write() const
	{
		typedef std::pair<typename Phrase<T>::type, uint> value_type;

		const typename PhraseCounter<T>::FrequencyMap& frequencies =
			processor_.getCounter(T()).getFrequencies();

		std::vector<value_type> phrases(frequencies.size());
		std::vector<value_type>::iterator phrasesIt = 
			std::remove_copy_if(
				frequencies.begin(),
				frequencies.end(),
				phrases.begin(),
				Remover<T>(REMOVER_THRESHOLD));
		phrases.erase(phrasesIt, phrases.end());
		std::sort(phrases.begin(), phrases.end(),
			Comparator<typename Phrase<T>::type>());
		
		os_ << phrases.size() << "\n";
		// ostream_iterator won't work here
		for (std::vector<value_type>::const_iterator it = phrases.begin();
			it != phrases.end(); ++it)
		{
			os_ << *it << "\n";
		}
	}

private:
	std::ostream& os_;
	const Processor& processor_;
};

void writeAnswer(const char* fileName, const Processor& processor)
{
	std::ofstream fout(fileName);
	if (!fout.is_open()) {
		std::cerr << "Cannot open output file.\n";
		return;
	}
	fout << processor.totalWordsNumber() << std::endl;
	Writer<WN::Single>(fout, processor).write();
	Writer<WN::Pair>(fout, processor).write();
	fout.close();
}


void calculate(const char* fileName, Processor* processor)
{
	FILE* fin;
	if (fopen_s(&fin, fileName, "r") != 0) {
		printf("Cannot open %s for reading", fileName);
		return;
	}
	unsigned int version;
	fscanf_s(fin, "%d", &version);
	assert(version == 1);
	char buf[MAX_LINE_SIZE];
	fgets(buf, MAX_LINE_SIZE, fin);
	while (true) {
		fgets(buf, MAX_LINE_SIZE, fin);
		if (feof(fin)) {
			break;
		}
		string title(buf);
		fgets(buf, MAX_LINE_SIZE, fin);
		string text(buf);
		processor->process(text);
	}
	fclose(fin);
}

template<class T>
void readBadPhrases(const char* fileName,
	std::set<typename Phrase<T>::type>* badPhrases)
{
	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		std::cerr << "Cannot open bad phrases file " << string(fileName) << "\n";
	}
	fin >> std::ws;
	badPhrases->clear();
	while (!fin.eof()) {
		typename Phrase<T>::type phrase;
		fin >> phrase >> std::ws;
		badPhrases->insert(phrase);
	}
	fin.close();
}

int main()
{
	const string path = "c:\\ys\\text\\";
	const string wikiFileName(path + "wiki.txt");
	const string badWordsFileName(path + "bad_words.txt");
	const string badPairFileName(path + "bad_pairs.txt");
	const string outputFileName(path + "stats");

	std::set<Word> badWords;
	std::set<Pair> badPairs;
	readBadPhrases<WN::Single>(badWordsFileName.c_str(), &badWords);
	readBadPhrases<WN::Pair>(badPairFileName.c_str(), &badPairs);

	badWords.insert("");
	Processor processor(badWords, badPairs);
	printf("Processing wiki...\n");
	calculate(wikiFileName.c_str(), &processor);
	printf("Calculating answer...\n");
	writeAnswer(outputFileName.c_str(), processor);
	return 0;
}

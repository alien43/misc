#include <common.h>

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/bind.hpp>

#include <fstream>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <cmath>


namespace WN = WordsNumber;
using std::string;
using std::vector;
typedef std::map<Word, uint> WordsMap;
typedef std::map<Pair, uint> PairsMap;
typedef std::vector<Pair> PairsVector;

const size_t RESULT_SIZE = 100;

struct CalcData {
	CalcData()
		: nWords(0)
	{}

	size_t nWords;
	WordsMap wordsMap;
	PairsMap pairsMap;
};

class ValueCalculator {
public:
	virtual ~ValueCalculator()
	{}

	virtual double get(const Pair&) const = 0;

	virtual string name() const = 0;
};

class Comparator {
public:
	// uses the ref given
	Comparator(const ValueCalculator& calculator)
		: calculator_(calculator)
	{}

	bool operator() (const Pair& lhs, const Pair& rhs) const
	{
		return calculator_.get(lhs) > calculator_.get(rhs);
	}

private:
	const ValueCalculator& calculator_;
};

void getKeyPhrases(const Comparator& comparator, const PairsVector& allPairs,
	size_t number, PairsVector* result)
{
	result->clear();
	const size_t nPairs = std::min(number, allPairs.size());
	result->resize(nPairs);
	std::partial_sort_copy(allPairs.begin(), allPairs.end(),
		result->begin(), result->end(), comparator);
}

template<class T>
void readPhrasesMap(std::istream& is,
	std::map<typename Phrase<T>::type, uint>* phrases)
{
	size_t nPhrases;
	is >> nPhrases;
	phrases->clear();
	for (size_t iPhrase = 0; iPhrase < nPhrases; ++iPhrase) {
		typename Phrase<T>::type phrase;
		is >> phrase;
		is >> (*phrases)[phrase];
	}
}

void readData(const char* fileName, CalcData* data)
{
	data->wordsMap.clear();
	data->pairsMap.clear();

	std::ifstream fin(fileName);
	if (!fin.is_open()) {
		std::cerr << "Could not open file " << string(fileName) << "\n";
		return;
	}
	std::cout << "Reading data...\n";
	fin >> data->nWords;
	readPhrasesMap<WN::Single>(fin, &(data->wordsMap));
	readPhrasesMap<WN::Pair>(fin, &(data->pairsMap));
	fin.close();
}

void processCalculator(const ValueCalculator& calculator,
	const string& path, const PairsVector& allPairs)
{
	std::cout << "Processing " << calculator.name() << "..." << std::endl;
	PairsVector result;
	getKeyPhrases(Comparator(calculator), allPairs, RESULT_SIZE, &result);

	std::ofstream fout(path + calculator.name());
	if (!fout.is_open()) {
		std::cerr << "Could not open file " << calculator.name() << "for writing\n";
		return;
	}
	// ostream_iterator won't work here
	for (PairsVector::const_iterator it = result.begin();
		it != result.end(); ++it)
	{
		fout << *it << "\n";
	}
	fout.close();
}


template<class Key, class Value>
void mapKeys2vector(const std::map<Key, Value>& map, std::vector<Key>* vector)
{
	vector->clear();
	vector->reserve(map.size());
	for (typename std::map<Key, Value>::const_iterator it = map.begin();
		it != map.end(); ++it)
	{
		vector->push_back(it->first);
	}
}

class FrequencyCalculator : public ValueCalculator {
public:
	// uses PairsMap ref given
	FrequencyCalculator(const PairsMap& pairsMap)
		: pairsMap_(pairsMap)
	{}

	double get(const Pair& pair) const
	{
		return static_cast<double>(pairsMap_.find(pair)->second);
	}

	string name() const
	{
		return "frequency";
	}

private:
	const PairsMap& pairsMap_;
};

class TStatCalculator : public ValueCalculator {
public:
	// uses the CalcData ref given
	TStatCalculator(const CalcData& data)
		: data_(data)
	{}

	double get(const Pair& pair) const
	{
		const uint pairFreq = data_.pairsMap.find(pair)->second;
		const uint firstFreq = data_.wordsMap.find(pair.first)->second;
		const uint secondFreq = data_.wordsMap.find(pair.second)->second;
		const double t = (pairFreq - 1.0 * firstFreq * secondFreq / data_.nWords) / 
			std::sqrt(1.0 * pairFreq);
		return -t;
	}

	string name() const
	{
		return "t-statistic";
	}

private:
	const CalcData& data_;
};

template<class T>
T sqr(T x)
{
	return x * x;
}

class ChiSquaredCalculator : public ValueCalculator {
public:
	// uses the CalcData ref given
	ChiSquaredCalculator(const CalcData& data)
		: data_(data)
	{}

	double get(const Pair& pair) const
	{
		/*
			word frequency can be less than pair frequency with this word
			because of deleting rare phrases in text processing
		*/
		const int O11 = static_cast<int>(data_.pairsMap.find(pair)->second);
		const int O12 = std::max(static_cast<int>(0),
			static_cast<int>(data_.wordsMap.find(pair.first)->second) - O11);
		const int O21 = std::max(static_cast<int>(0),
			static_cast<int>(data_.wordsMap.find(pair.second)->second) - O11);
		const int O22 = data_.nWords - O12 - O21 + O11;
		const double chisq = 1.0 * data_.nWords * sqr(O11 * O22 - O12 * O21)
			/ (O11 + O12) * (O11 + O21) * (O22 + O12) * (O22 + O21);
		return -chisq;
	}

	string name() const
	{
		return "chi-squared-test";
	}

private:
	const CalcData& data_;
};

class LikelihoodCalculator : public ValueCalculator {
public:
	// uses the CalcData ref given
	LikelihoodCalculator(const CalcData& data)
		: data_(data)
	{}

	double get(const Pair& pair) const
	{
		const size_t N = data_.nWords;
		const uint c12 = data_.pairsMap.find(pair)->second;
		const uint c1 = data_.wordsMap.find(pair.first)->second;
		const uint c2 = data_.wordsMap.find(pair.second)->second;
		const double p = 1.0 * c2 / N;
		const double p1 = 1.0 * c12 / c1;
		const double p2 = 1.0 * (c2 - c12) / (N - c1);
		const double logLambda = logL(c12, c1, p) + logL(c2 - c12, N - c1, p)
			- logL(c12, c1, p1) - logL(c2 - c12, N - c1, p2);
		return -2.0 * logLambda;
	}

	string name() const
	{
		return "likelihood";
	}

private:
	static double logL(uint n, uint k, double x)
	{
		return std::log(x) * k + std::log(1 - x) * (n - k);
	}

	const CalcData& data_;
};


int main()
{
	const string path = "c:\\ys\\text\\";
	const string statsFileName(path + "stats");
	std::auto_ptr<CalcData> data(new CalcData);
	readData(statsFileName.c_str(), data.get());

	std::auto_ptr<PairsVector> allPairs(new PairsVector());
	mapKeys2vector(data->pairsMap, allPairs.get());

	boost::ptr_vector<ValueCalculator> calculators;
	calculators.push_back(new FrequencyCalculator(data->pairsMap));
	calculators.push_back(new TStatCalculator(*data));
	calculators.push_back(new ChiSquaredCalculator(*data));
	calculators.push_back(new LikelihoodCalculator(*data));
	std::for_each(calculators.begin(), calculators.end(),
		boost::bind(processCalculator, _1, path, *allPairs));
	return 0;
}

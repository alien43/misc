#ifndef PHRASE_COUNTER_H_INCLUDED
#define PHRASE_COUNTER_H_INCLUDED

#include "document.h"

#include <boost/scoped_ptr.hpp>

#include <map>

struct CounterCleanupConfig {
	bool isNeeded;
	size_t deleteThreshold;
	size_t period;
};

class PhraseCounter {
public:
	typedef Document::Element Element;
	typedef std::map<Element, uint> FrequencyMap;
	typedef FrequencyMap::value_type value_type;

	explicit PhraseCounter(const CounterCleanupConfig& cleanupConfig);

	void add(const Element& element);

	const FrequencyMap& getFrequencies() const;

	void clear();

private:
	void doCleanup();

	size_t numberOfPhrases_;
	boost::scoped_ptr<FrequencyMap> freqs_;
	CounterCleanupConfig cleanupConfig_;
};

#endif //PHRASE_COUNTER_H_INCLUDED

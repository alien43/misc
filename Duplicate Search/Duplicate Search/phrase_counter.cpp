#include "phrase_counter.h"

PhraseCounter::PhraseCounter(const CounterCleanupConfig& cleanupConfig)
	: freqs_(new FrequencyMap), cleanupConfig_(cleanupConfig)
{}

void PhraseCounter::add(const Element& element)
{
	// increase counter, inserting new element at first, if needed
	++((freqs_->insert(std::make_pair(element, 0))).first->second);

	++numberOfPhrases_;
	if (cleanupConfig_.isNeeded &&
		(numberOfPhrases_% cleanupConfig_.period == 0))
	{
		doCleanup();
	}
}

const PhraseCounter::FrequencyMap& PhraseCounter::getFrequencies() const
{
	return *freqs_;
}

void PhraseCounter::clear()
{
	freqs_->clear();
}


void PhraseCounter::doCleanup()
{
	for (FrequencyMap::iterator it = freqs_->begin();
		it != freqs_->end();)
	{
		if (it->second < cleanupConfig_.deleteThreshold) {
			it = freqs_->erase(it);
		} else {
			++it;
		}
	}
}

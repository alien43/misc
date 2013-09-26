#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include "document.h"
#include "bad_phrases.h"
#include "vocabulary.h"

#include <boost/tokenizer.hpp>

#include <deque>
#include <algorithm>

template<size_t N>
class Processor {
public:
	typedef typename Phrase<N>::type PhraseType;
	typedef typename Vocabulary<N>::type VocabularyType;
	typedef typename BadPhrases<N>::type BadPhrasesType;

	// Uses reference to the bad phrases sets
	Processor(const BadPhrasesType& badPhrases, VocabularyType* vocabulary)
		: nPhrases_(0), badPhrases_(badPhrases), vocabulary_(vocabulary)
	{}

	/*
		If the document is null, works in vocabulary filling mode.
		Does not clear presenting text in the document.
	*/
	
	void process(const std::string& text, Document* document = 0)
	{
		std::deque<Word> context;

		typedef boost::tokenizer< boost::char_separator<char> > tokenizer;
		boost::char_separator<char> separators(" ");
		tokenizer tok(text, separators);
		for (tokenizer::iterator it = tok.begin();
			it != tok.end(); ++it)
		{
			context.push_back(*it);
			if (context.size() > N) {
				context.pop_front();
			}
			if (context.size() == N) {
				PhraseType phrase;
				std::copy(context.begin(), context.end(), phrase.begin());
				if (isGood(phrase)) {
					const typename VocabularyType::iterator itPhrase = vocabulary_->find(phrase);
					Id id;
					if (itPhrase != vocabulary_->end()) {
						id = itPhrase->second;
					} else {
						id = vocabulary_->size();
						vocabulary_->insert(std::make_pair(phrase, id));
					}
					if (document) {
						document->words.push_back(id);
					}
				}
			}
		}
	}

private:
	bool isGood(const PhraseType& phrase) const
	{
		return (badPhrases_.count(phrase) == 0);
	}

	size_t nPhrases_;
	const BadPhrasesType& badPhrases_;
	VocabularyType* const vocabulary_;
};


#endif

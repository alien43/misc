#include "handlers.h"
#include "phrase_counter.h"

#include <boost/bind.hpp>

#include <algorithm>
#include <memory>


TraitHandler::TraitHandler(const RPHash* hasher)
	:traits_(new std::vector<DocumentTrait>), hasher_(hasher)
{
}

TraitHandler::~TraitHandler()
{
}


void TraitHandler::handle(std::auto_ptr<Document> document)
{
	if (document->words.size() < MIN_WORDS_NUMBER) {
		return;
	}
	CounterCleanupConfig cleanupConfig;
	cleanupConfig.isNeeded = false;
	PhraseCounter counter(cleanupConfig);
	std::for_each(document->words.begin(), document->words.end(),
		boost::bind(&PhraseCounter::add, boost::ref(counter), _1));

	const RPHash::DocumentVector& frequencies(counter.getFrequencies());
	DocumentTrait newDoc;
	newDoc.title = document->title;
	newDoc.hash = hasher_->get(frequencies);
	traits_->push_back(newDoc);
}

const DocumentTrait& TraitHandler::traitAt(size_t index) const
{
	return traits_->at(index);
}

size_t TraitHandler::size() const
{
	return traits_->size();
}

SaveHandler::SaveHandler(const std::vector<std::string>& titles)
: titles_(titles), documents_()
{
	std::sort(titles_.begin(), titles_.end());
}

void SaveHandler::handle(std::auto_ptr<Document> document)
{
	const std::string& title = document->title;
	if (std::binary_search(titles_.begin(), titles_.end(), title)
		&& documents_.count(title) == 0)
	{
		documents_.insert(title, document);
	}
}

const Document& SaveHandler::find(const std::string& title) const
{
	return documents_.at(title);
}

size_t SaveHandler::size() const
{
	return documents_.size();
}

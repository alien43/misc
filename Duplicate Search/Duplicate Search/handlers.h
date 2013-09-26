#ifndef HANDLERS_H_INCLUDED
#define HANDLERS_H_INCLUDED

#include "document_handler.h"
#include "random_projection_hash.h"

#include <boost/scoped_ptr.hpp>
#include <boost/ptr_container/ptr_map.hpp>

#include <vector>
#include <map>

struct DocumentTrait {
	std::string title;
	RPHash::ResultType hash;
};

class RPHash;

class TraitHandler : public DocumentHandler {
public:
	// uses RPHash object given by pointer
	TraitHandler(const RPHash* hasher);
	~TraitHandler();

	void handle(std::auto_ptr<Document>);
	
	size_t size() const;
	const DocumentTrait& traitAt(size_t index) const;

private:
	static const uint MIN_WORDS_NUMBER = 50;

	boost::scoped_ptr< std::vector<DocumentTrait> > traits_;
	const RPHash* const hasher_;
};


class DummyHandler : public DocumentHandler {
public:
	void handle(std::auto_ptr<Document>)
	{}

};

class SaveHandler : public DocumentHandler {
public:
	SaveHandler(const std::vector<std::string>& titles);

	void handle(std::auto_ptr<Document>);

	const Document& find(const std::string& title) const;

	size_t size() const;

private:
	typedef boost::ptr_map<std::string, Document> DocumentsMap;
	std::vector<std::string> titles_;
	DocumentsMap documents_;
};

#endif //HANDLERS_H_INCLUDED

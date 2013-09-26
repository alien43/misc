#ifndef DOCUMENT_HANDLER_H_INCLUDED
#define DOCUMENT_HANDLER_H_INCLUDED

#include "document.h"

#include <memory>

class DocumentHandler {
public:
	virtual ~DocumentHandler()
	{}

	virtual void handle(std::auto_ptr<Document>) = 0;
};

#endif //DOCUMENT_HANDLER_H_INCLUDED

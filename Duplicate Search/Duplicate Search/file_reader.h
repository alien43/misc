#ifndef FILE_READER_H_INCLUDED
#define FILE_READER_H_INCLUDED

#include "document_handler.h"
#include "processor.h"

#include <boost/algorithm/string.hpp>

#include <cstdio>
#include <memory>
#include <cassert>

#include <iostream>
#include <fstream>
#include <vector>

const size_t MAX_LINE_SIZE = 1000000;

// if handler is 0, then works in vocabulary filling mode
template<size_t N>
void processFile(const std::string& fileName, Processor<N>* processor,
	DocumentHandler* handler = 0)
{
	FILE* fin;
	if (fopen_s(&fin, fileName.c_str(), "r") != 0) {
		printf("Cannot open %s for reading", fileName.c_str());
		return;
	}
	unsigned int version;
	fscanf_s(fin, "%d", &version);
	assert(version == 1);

	char buf[MAX_LINE_SIZE];
	if (!fgets(buf, MAX_LINE_SIZE, fin)) {
		fclose(fin);
		return;
	}

	std::auto_ptr<Document> document;
	while (true) {
		document.reset(new Document);
		if (!fgets(buf, MAX_LINE_SIZE, fin) || feof(fin)) {
			break;
		}
		document->title.assign(buf);
		boost::trim(document->title);
		string text;
		while (fgets(buf, MAX_LINE_SIZE, fin) && !feof(fin)) {
			string strBuf(buf);
			text += strBuf;
			if (strBuf.size() + 1 < MAX_LINE_SIZE) {
				break;
			}
		}

		if (handler == 0) {
			processor->process(text);
		} else {
			processor->process(text, document.get());
			handler->handle(document);
		}
	}
	fclose(fin);
}


#endif //FILE_READER_H_INCLUDED

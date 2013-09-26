#include "similar.h"
#include "myfinder.h"
#include "query.h"

#include <boost/bind.hpp>

#include <string>
#include <iostream>
#include <algorithm>
#include <memory>

using std::string;

#define DEBUG_MODE

void printUsage(const string& programName)
{
	std::cout <<
		"Usage:\n" << programName <<
		" <file with queries filename>"
		" <file with collection filename>"
		" <output directory>"
		"\n";
}

int main(int argc, char* argv[])
{
#ifdef DEBUG_MODE
	const string path = "C:\\YS\\ip\\shapes\\";
	const string queriesFileName(path + "queries.txt");
	const string collectionFileName(path + "images.txt");
	const string outputDirectory(path + "result\\");
#else
	const string programName(argv[0]);
	if (argc != 4) {
		printUsage(programName);
	}

	const string queriesFileName(argv[1]);
	const string collectionFileName(argv[2]);
	const string outputDirectory(argv[3]);
#endif

	std::auto_ptr<MyFinder> similarFinder(new MyFinder);
	SimilarFinder::setInstance(similarFinder.get());

	std::cout << "Creating collection...\n";
	Collection collection = createCollection(collectionFileName);
	std::cout << "Collection created successfully.\nPerfoming queries...\n";
	QueriesList queries = createQueriesList(queriesFileName);
	for (size_t iQuery = 0; iQuery < queries.size(); ++iQuery)
	{
		const Query& query = queries[iQuery];
		const string outputFileName(outputDirectory + query.name());
		printSimilar(query, collection, outputFileName);
	}
	std::cout << "Finished." << std::endl;
	return 0;
}

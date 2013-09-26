#include "similar_finder.h"
#include "handlers.h"

#include <map>
#include <memory>

using std::vector;

typedef std::map<RPHash::ResultType, Indexes> HashesMap;

void findSimilar(const TraitHandler& handler,
	vector<Indexes>* result)
{
	result->clear();
	std::auto_ptr<HashesMap> hashes(new HashesMap);
	for (size_t iDoc = 0; iDoc < handler.size(); ++iDoc) {
		const RPHash::ResultType& hash = handler.traitAt(iDoc).hash;
		(*hashes)[hash].push_back(iDoc);
	}

	for (HashesMap::const_iterator it = hashes->begin();
		it != hashes->end(); ++it)
	{
		if (it->second.size() > 1) {
			result->push_back(it->second);
		}
	}
}

#ifndef QUERY_H_INCLUDED
#define QUERY_H_INCLUDED

#include "path.h"

#include <vector>

class Query {
public:
	Query(const std::string& queryFileName);

	const Path& path() const;

	const std::string& name() const;

private:
	Path path_;
	std::string name_;
};

typedef std::vector<Query> QueriesList;

QueriesList createQueriesList(const std::string& fileName);

#endif //QUERY_H_INCLUDED

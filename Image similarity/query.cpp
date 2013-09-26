#include "query.h"
#include "common.h"

#include <iostream>
#include <fstream>

using std::string;

Query::Query(const string& queryFileName)
{
	std::ifstream fin(queryFileName.c_str());
	if (!fin.is_open()) {
		THROW_EX("Cannon open query file " + queryFileName);
	}
	string pathWithoutExtention;
	fin >> pathWithoutExtention;
	fin.close();
	path_ = Path(pathWithoutExtention);
	name_ = Path(queryFileName.substr(0, queryFileName.length() - 4)).name();
}

const Path& Query::path() const
{
	return path_;
}

const string& Query::name() const
{
	return name_;
}

QueriesList createQueriesList(const string& fileName)
{
	std::ifstream fin(fileName.c_str());
	if (!fin.is_open()) {
		THROW_EX("Could not open queries file " + fileName);
	}
	QueriesList result;
	while (!fin.eof()) {
		fin >> std::ws;
		string path;
		std::getline(fin, path);
		fin >> std::ws;
		result.push_back(Query(path));
	}
	fin.close();
	return result;
}

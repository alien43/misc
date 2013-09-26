#include "collection.h"
#include "common.h"

#include <iostream>
#include <fstream>

using std::string;

void Collection::addImage(const Path& path)
{
	traits_.push_back(new ImageTrait(path));
}

size_t Collection::size() const
{
	return traits_.size();
}

const ImageTrait& Collection::traitAt(size_t index) const
{
	return traits_.at(index);
}

Collection createCollection(const string& collectionFileName)
{
	std::ifstream fin(collectionFileName.c_str());
	if (!fin.is_open()) {
		THROW_EX("Could not open collection file " + collectionFileName);
	}
	Collection result;
	while (!fin.eof()) {
		fin >> std::ws;
		string pathWithoutExtention;
		std::getline(fin, pathWithoutExtention);
		fin >> std::ws;
		Path path(pathWithoutExtention);
		result.addImage(path);
	}
	fin.close();
	return result;
}

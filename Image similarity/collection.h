#ifndef COLLECTION_H_INCLUDED
#define COLLECTION_H_INCLUDED

#include "image_trait.h"

#include <boost/ptr_container/ptr_vector.hpp>

#include <string>
#include <vector>

class Collection {
public:
	Collection()
	{}

	void addImage(const Path& path);

	size_t size() const;
	const ImageTrait& traitAt(size_t index) const;

private:
	boost::ptr_vector<ImageTrait> traits_;
};

Collection createCollection(const std::string& collectionFileName);

#endif //COLLECTION_H_INCLUDED


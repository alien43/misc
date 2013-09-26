#ifndef PATH_H_INCLUDED
#define PATH_H_INCLUDED

#include <string>

class Path {
public:
	Path()
		{}
	Path(const std::string& pathWithoutExtention);

	// Full path
	std::string fullPath() const;

	// Path to file without extension ".jpg"
	const std::string& pathWithoutExtention() const;

	// Name of the file without path and extension
	std::string name() const;

private:
	std::string pathWithoutExtention_;
};

#endif //PATH_H_INCLUDED

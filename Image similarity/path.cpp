#include "path.h"

using std::string;

Path::Path(const string& pathWithoutExtention)
	:pathWithoutExtention_(pathWithoutExtention)
{
}

string Path::fullPath() const
{
	return pathWithoutExtention() + ".jpg";
}

const string& Path::pathWithoutExtention() const
{
	return pathWithoutExtention_;
}

string Path::name() const
{
	const string& path = pathWithoutExtention();
	size_t startPos = path.find_last_of('\\');
	if (startPos == string::npos) {
		startPos = 0;
	} else {
		++startPos;
	}
	return path.substr(startPos);
}

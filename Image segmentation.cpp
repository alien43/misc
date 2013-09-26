#include <cv.h>
#include <highgui.h>
#include <ml.h>

#include <boost/lexical_cast.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <iterator>
#include <utility>
#include <algorithm>
#include <map>

using namespace std;
using namespace cv;

typedef unsigned int uint;
typedef vector<string> NamesList;

struct Sample {
	Sample(const string& imgFileName, const string& gdFileName, double w)
		: imageFileName(imgFileName), groundTruthFileName(gdFileName), weight(w)
	{}

	string imageFileName;
	string groundTruthFileName;
	double weight;
};

typedef vector<Sample> SamplesList;

const string EXT = ".bmp";

struct Path {
	static string path()
	{
		return "C:\\YS\\ip2\\Collection\\";
	}
	static string imagesPath()
	{
		return path() + "Images\\";
	}
	static string groundTruthPath()
	{
		return path() + "GroundTruth\\";
	}
	static string resultPath()
	{
		return path() + "Result\\";
	}
	static string hqPath()
	{
		return path() + "SegmentationsGTHighQuality\\";
	}
	static string namesFileName()
	{
		return path() + "all_images";
	}
	static string hqListFileName()
	{
		return path() + "hq";
	}
};

void readNames(const string& fileName, NamesList* names)
{
	ifstream fin(fileName.c_str());
	if (!fin.is_open()) {
		throw runtime_error("Cannot open file " + fileName);
	}
	names->clear();
	copy(istream_iterator<string>(fin), istream_iterator<string>(), back_inserter(*names));
	fin.close();
	sort(names->begin(), names->end());
}


uint imageCategory(const string& name)
{
	return boost::lexical_cast<uint>(name.substr(0, name.find_first_of('_')));
}

uint imageNumber(const string& name)
{
	const size_t start = name.find_first_of('_') + 1;
	return boost::lexical_cast<uint>(name.substr(start, name.find_last_of('_') - start));
}

void generateSamples(SamplesList* learning, SamplesList* testing)
{
	NamesList names;
	readNames(Path::hqListFileName(), &names);
	learning->clear();
	for (NamesList::const_iterator it = names.begin(); it != names.end(); ++it) {
		learning->push_back(Sample(
			Path::hqPath() + *it + EXT,
			Path::hqPath() + *it + "_HQGT" + EXT,
			2.0));
	}

	readNames(Path::namesFileName(), &names);
	map<uint, size_t> categorySize;
	for (NamesList::const_iterator it = names.begin(); it != names.end(); ++it) {
		++(*((categorySize.insert(make_pair(imageCategory(*it), 0))).first)).second;
	}

	testing->clear();
	for (NamesList::const_iterator it = names.begin(); it != names.end(); ++it) {
		const bool isLearning = (2 * imageNumber(*it) <= categorySize[imageCategory(*it)]);
		Sample sample(
			Path::imagesPath() + *it + EXT,
			"",	
			1.0);
		if (isLearning) {
			sample.groundTruthFileName = Path::groundTruthPath() + *it + "_GT" + EXT,
			learning->push_back(sample);
		} else {
			sample.groundTruthFileName = Path::resultPath() + *it + EXT;
			testing->push_back(sample);
		}
	}
}

int main() 
{
	SamplesList learning, testing;
	generateSamples(&learning, &testing);

	Mat im = imread(learning.front().imageFileName);
	imshow("preved", im);

	waitKey();
	
	return 0;
}

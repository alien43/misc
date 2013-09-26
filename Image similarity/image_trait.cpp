#include "image_trait.h"
#include "common.h"

#include "debug_helper.h"

#include <highgui.h>

#include <boost/bind.hpp>

#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>
#include <cmath>

using namespace cv;

using std::vector;

ImageTrait::~ImageTrait()
{
}

ImageTrait::ImageTrait(const Path& path)
	:path_(path)
{
	build();
}

const Path& ImageTrait::path() const
{
	return path_;
}

void ImageTrait::build()
{
	Mat src = imread(path().fullPath());
	if (!src.data) {
		THROW_EX("Could not load image " + path().fullPath());
	}
	Mat hsv;
	cvtColor(src, hsv, CV_BGR2HSV);
	vector<Mat> colors;

	// split(hsv, colors);
	// const Mat& hue = colors.at(0);

	std::cout << "Building " << path_.name() << std::endl;
	color_ = ColorTrait(hsv);
	shape_ = ShapeTrait(hsv);
	texture_ = TextureTrait(hsv);
}

ColorTrait::ColorTrait(const Mat& hsv)
{
	int hbins = 16, sbins = 32;
	int histSize[] = {hbins, sbins};
	float hranges[] = {0, 360};
	float sranges[] = {0, 256};
	const float* ranges[] = {hranges, sranges};

	int channels[] = {0, 1};
	calcHist(&hsv, 1, channels, Mat(), histogram, 1, histSize, ranges,
		true, false);

	// printMatND<float>(std::cout, histogram);
}


ShapeTrait::ShapeTrait(const Mat& hsv)
{
	Mat rgb;
	cvtColor(hsv, rgb, CV_HSV2RGB);
	Mat gray;
	cvtColor(rgb, gray, CV_RGB2GRAY);
	Moments moments(cv::moments(gray));
	double huInv[numberOfHuInvariants];
	HuMoments(moments, huInv);
	huInvariants.assign(huInv, huInv + numberOfHuInvariants - 1);

	std::transform(huInvariants.begin(), huInvariants.end(), huInvariants.begin(),
		boost::bind(std::multiplies<double>(), _1, 5.0));

	// std::copy(huInvariants.begin(), huInvariants.end(),
	//	std::ostream_iterator<double>(std::cout, " "));
}

void countGLCM(const Mat& src, Mat& dst, const Vec<int, 2>& r)
{
	if (src.type() != CV_8UC1) {
		THROW_EX("Can only work with 1 byte unsigned single channel images");
	}
	const size_t L = 1 << (8 * sizeof(uchar));
	dst = Mat(L, L, CV_32SC1, Scalar(0));
	const Rect matRect(0, 0, src.cols, src.rows);
	for (int iRow = 0; iRow < src.rows; ++iRow) {
	for (int iCol = 0; iCol < src.cols; ++iCol) {
		Point to(iCol + r[0], iRow + r[1]);
		if (matRect.contains(to)) {
			++dst.at<int>(src.at<uchar>(iRow, iCol), src.at<uchar>(to));
		}
	}
	}
}

template<class T>
inline T sqr(T x)
{
	return x * x;
}

TextureTrait::Features::Features(const Mat& GLCM)
{
	if (GLCM.rows != GLCM.cols) {
		THROW_EX("GLCM must be a square matrix");
	}
	const int L = GLCM.rows;
	ASM = 0.0; CON = 0.0; IDM = 0.0; ENT = 0.0; COR = 0.0;
	for (int iRow = 0; iRow < L; ++iRow) {
	for (int iCol = 0; iCol < L; ++iCol) {
		const int v = GLCM.at<int>(iRow, iCol);
		ASM += sqr(1.0 * v);
		const int n = iRow - iCol;
		CON += 1.0 * sqr(n) * v;
		IDM += 1.0 * v / (1 + sqr(n));
		if (v != 0) {
			ENT += -std::log(1.0 * v) * v;
		}
	}
	// COR could be calculated later
	}
}

TextureTrait::TextureTrait(const Mat& hsv)
{
	std::vector< Vec<int, 2> > rs;
	rs.push_back(Vec<int, 2>(1, 0));
	rs.push_back(Vec<int, 2>(1, 1));
	rs.push_back(Vec<int, 2>(0, 1));
	rs.push_back(Vec<int, 2>(-1, 1));

	Mat rgb;
	cvtColor(hsv, rgb, CV_HSV2RGB);
	Mat gray;
	cvtColor(rgb, gray, CV_RGB2GRAY);
	features.reserve(rs.size());
	for (size_t ir = 0; ir < rs.size(); ++ir) {
		Mat GLCM;
		countGLCM(gray, GLCM, rs[ir]);
		features.push_back(Features(GLCM));
	}

	printTextureTraitFeatures(std::cout, features.front());
}

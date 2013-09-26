#include "myfinder.h"
#include "common.h"
#include "image_trait.h"

#include <cv.h>
#include <highgui.h>

#include <cmath>
#include <algorithm>
#include <iostream>

using namespace cv;
using namespace std;


double MyFinder::find(const ImageTrait& imgQuery, const ImageTrait& imgCollected) const
{
	double colorResult = compareHist(
		imgQuery.color().histogram, imgCollected.color().histogram,
		CV_COMP_INTERSECT);
	colorResult /= sum(imgQuery.color().histogram)[0];
	// cout << "between " << imgQuery.path().name() << " and " << imgCollected.path().name() << ": "
	//	<< colorResult  << endl;

	const double result = colorResult;
	return result;
}

double MyFinder::threshold() const
{
	return 0.5;
}

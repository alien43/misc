#include "simple_resizer.h"
#include "filter_factory.h"
#include "filter.h"

#include <iostream>
#include <vector>
#include <memory>

using cv::Mat;
using cv::Size;

using std::vector;


IImageResizer* CreateSimpleResizer()
{
	return new SimpleResizer;
}

void SimpleResizer::Resize(const Mat& src, Mat& dst, Size newSize) const
{
	std::auto_ptr<Filter> filter(FilterFactory::getFunctionFilter("lanczos", 2.0));

	vector<Mat> srcPlanes;
	cv::split(src, srcPlanes);
	vector<Mat> dstPlanes(srcPlanes.size());
	for (size_t iPlane = 0; iPlane < dstPlanes.size(); ++iPlane) {
		dstPlanes[iPlane] = Mat(newSize, srcPlanes[iPlane].type());
		const Mat& srcp = srcPlanes[iPlane];
		Mat& dstp = dstPlanes[iPlane];
		filter->apply(srcp, dstp);
	}

	cv::merge(dstPlanes, dst);

	// cv::resize(src, dst, newSize, 0.0, 0.0, cv::INTER_LINEAR);
}

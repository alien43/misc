#include "filter.h"
#include "function.h"

#include <cmath>
#include <limits>

using cv::Mat;
using cv::Size;

void Filter::apply(const Mat& src, Mat& dst) const
{
	const Size srcSize = src.size();
	const Size dstSize = dst.size();
	const double xRatio = 1.0 * srcSize.width / dstSize.width;
	const double yRatio = 1.0 * srcSize.height / dstSize.height;

	for (int x = 0; x < dstSize.width;  ++x) {
	for (int y = 0; y < dstSize.height; ++y) {
		const double srcX = xRatio * x;
		const double srcY = yRatio * y;
		double value = at(src, srcY, srcX);
		if (value > std::numeric_limits<etype>::max()) {
			value = std::numeric_limits<etype>::max();
		} else if (value < std::numeric_limits<etype>::min()) {
			value = std::numeric_limits<etype>::min();
		}
		dst.at<etype>(y, x) = cv::saturate_cast<etype>(value);
	}
	}
}


FunctionFilter::FunctionFilter(Function* function)
: function_(function)
{
}

double FunctionFilter::at(const Mat& src, double y, double x) const
{
	const double EPS = Function::EPS;
	const Function::Domain domain = function_->domain();
	Function::Domain curDomain(x + domain.x, y + domain.y,
		domain.width, domain.height);
	double weightSum = 0.0;
	double value = 0.0;
	for (int cx = static_cast<int>(std::ceil(curDomain.x - EPS));
		cx <= static_cast<int>(std::floor(curDomain.x + curDomain.width + EPS));
		++cx) {
	for (int cy = static_cast<int>(std::ceil(curDomain.y - EPS));
		cy <= static_cast<int>(std::floor(curDomain.y + curDomain.height + EPS));
		++cy) {

		if (cx < 0 || cx >= src.size().width || cy < 0 || cy >= src.size().height) {
			continue;
		}
		const etype valueAt = src.at<etype>(cy, cx);
		const double weight = function_->at(y - cy, x - cx);
		value += weight * valueAt;
		weightSum += weight;
	}
	}
	if (std::abs(weightSum) > EPS) {
		value /= weightSum;
	}
	return value;
}

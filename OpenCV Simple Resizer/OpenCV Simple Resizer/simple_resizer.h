#ifndef SIMPLE_RESIZER_H_INCLUDED
#define SIMPLE_RESIZER_H_INCLUDED

#include "image_resizer.h"

class SimpleResizer : public IImageResizer {
public:
	SimpleResizer()
	{}

	void Resize(const cv::Mat& src, cv::Mat& dst, cv::Size newSize) const;
};

IImageResizer* CreateSimpleResizer();

#endif //SIMPLE_RESIZER_H_INCLUDED

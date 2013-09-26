#ifndef IMAGE_RESIZER_H_INCLUDED
#define IMAGE_RESIZER_H_INCLUDED

#include <cv.h>

//base class 
class IImageResizer {
public:
    IImageResizer() {}
    virtual ~IImageResizer() {}

    virtual void Resize(const cv::Mat& src, cv::Mat& dst,
		cv::Size newSize) const = 0;
};


#endif //IMAGE_RESIZER_H_INCLUDED

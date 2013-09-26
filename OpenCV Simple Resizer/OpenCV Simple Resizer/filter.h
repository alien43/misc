#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED


#include <cv.h>

#include <memory>

typedef uchar etype;

class Filter {
public:

	virtual ~Filter()
	{}

	// new size is taken from dst
	void apply(const cv::Mat& src, cv::Mat& dst) const;

	virtual double at(const cv::Mat&, double y, double x) const = 0;
};


class Function;

class FunctionFilter : public Filter {
public:
	FunctionFilter(Function*);

	virtual double at(const cv::Mat&, double y, double x) const;

private:
	std::auto_ptr<Function> function_;
};

#endif //FILTER_H_INCLUDED

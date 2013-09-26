#ifndef IMAGE_TRAIT_H_INCLUDED
#define IMAGE_TRAIT_H_INCLUDED

#include "path.h"

#include <cv.h>

#include <vector>

struct ColorTrait {
	ColorTrait()
	{}
	ColorTrait(const cv::Mat&);
	
	cv::MatND histogram;
};

struct ShapeTrait {
	ShapeTrait()
	{}
	ShapeTrait(const cv::Mat&);

	static const unsigned int numberOfHuInvariants = 7;

	std::vector<double> huInvariants;
};

struct TextureTrait {
	struct Features {
		Features(const cv::Mat& GLCM);

		double ASM;
		double CON;
		double IDM;
		double ENT;
		double COR;
	};

	TextureTrait()
	{}

	TextureTrait(const cv::Mat&);

	std::vector<Features> features;
};

class ImageTrait {
public:
	ImageTrait(const Path& path);
	~ImageTrait();

	const Path& path() const;

	const ColorTrait& color() const
	{
		return color_;
	}

	const ShapeTrait& shape() const
	{
		return shape_;
	}

	const TextureTrait& texture() const
	{
		return texture_;
	}


private:
	void build();

	Path path_;

	ColorTrait color_;
	ShapeTrait shape_;
	TextureTrait texture_;
};

#endif //IMAGE_TRAIT_H_INCLUDED

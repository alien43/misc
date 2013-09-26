#ifndef DEBUG_HELPER_H_INCLUDED
#define DEBUG_HELPER_H_INCLUDED

#include <iostream>
#include <cv.h>

template<class Type>
void printMatND(std::ostream& os, const cv::MatND& mat)
{
	if (mat.dims > 2) {
		os << "more than 2D matrix\n";
		return;
	}
	if (mat.dims == 1) {
		for (int i0 = 0; i0 < mat.size[0]; ++i0) {
			os << mat.at<Type>(i0) << ' ';
		}
		os << "\n";
	} else {
		for (int i0 = 0; i0 < mat.size[0]; ++i0) {
			for (int i1 = 0; i1 < mat.size[1]; ++i1) {
				os << mat.at<Type>(i0, i1) << ' ';
			}
			os << "\n";
		}
	}
	os << std::endl;
}


void printTextureTraitFeatures(std::ostream& os,
	const TextureTrait::Features& fs)
{
	os << "ASM = " << fs.ASM << "; CON = " << fs.CON << "; IDM = " << fs.IDM
		<< "; ENT = " << fs.ENT << "; COR = " << fs.COR << "\n";
}

#endif //DEBUG_HELPER_H_INCLUDED

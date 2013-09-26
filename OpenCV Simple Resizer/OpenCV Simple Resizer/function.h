#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED

#include <cv.h>

#include <boost/function.hpp>

class Function {
public:
	// area, outside which the function is zero
	typedef cv::Rect_<double> Domain;
	static const double EPS;


	explicit Function(const Domain&);

	const Domain& domain() const;

	virtual double at(double y, double x) const = 0;

private:
	Domain domain_;
};


typedef boost::function<double (double x)> UnaryFunction;
typedef boost::function<double (double y, double x)> BinaryFunction;

// FunctionType is supposed to be either UnaryFunction or BinaryFunction
template<class FunctionType>
class FunctionWrapper : public Function {
public:
	FunctionWrapper(const Function::Domain&, const FunctionType&);

	double at(double y, double x) const;

private:
	FunctionType function_;
};

#define FUNCTION_TPL
#include "function-tpl.h"
#undef FUNCTION_TPL

#endif //FUNCTION_H_INCLUDED

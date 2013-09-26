#define _USE_MATH_DEFINES

#include "filter_factory.h"
#include "filter.h"
#include "function.h"

#include <boost/bind.hpp>

#include <cmath>

double trivial_(double x)
{
	if (std::abs(x) < 1) {
		return 1.0;
	} else {
		return 0.0;
	}
}

double mean_(double x)
{
	return 1.0 - x;
}

double lanzcos_(double x, double a)
{
	if (std::abs(x) < Function::EPS) {
		return 1.0;
	} else {
		return a * std::sin(M_PI * x) * std::sin(M_PI * x / a)
			/ (M_PI * M_PI * x * x);
	}
}

FunctionFilter* FilterFactory::getFunctionFilter(const std::string& name, double a)
{
	if (name == "trivial") {
		return new FunctionFilter(new FunctionWrapper<UnaryFunction>(
			Function::Domain(-1.0, -1.0, 2.0, 2.0), UnaryFunction(trivial_)));
	} else if (name == "mean") {
		return new FunctionFilter(new FunctionWrapper<UnaryFunction>(
			Function::Domain(-1.0, -1.0, 2.0, 2.0), UnaryFunction(mean_)));
	} else if (name == "lanczos") {
		return new FunctionFilter(new FunctionWrapper<UnaryFunction>(
			Function::Domain(-a, -a, 2.0 * a, 2.0 * a),
			boost::bind(lanzcos_, _1, a)));
	}
	return 0;
}
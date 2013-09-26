#ifndef FILTER_FACTORY_H_INCLUDED
#define FILTER_FACTORY_H_INCLUDED

#include <string>

class FunctionFilter;

class FilterFactory {
public:
	static FunctionFilter* getFunctionFilter(const std::string& name, double a = 0.0);
};

#endif //FILTER_FACTORY_H_INCLUDED

#ifndef FUNCTION_TPL
#error "Direct inclusion of function-tpl.h is forbidden, include function.h instead"
#endif

template<class FunctionType> inline
FunctionWrapper<FunctionType>::FunctionWrapper(const Function::Domain& dom,
	const FunctionType& function)
	: Function(dom), function_(function)
{
}

template<> inline 
double FunctionWrapper<UnaryFunction>::at(double y, double x) const
{
	return function_(y) * function_(x);
}

template<> inline
double FunctionWrapper<BinaryFunction>::at(double y, double x) const
{
	return function_(y, x);
}


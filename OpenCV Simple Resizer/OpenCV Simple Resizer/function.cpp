#include "function.h"

const double Function::EPS = 1E-7;

Function::Function(const Function::Domain& dom)
: domain_(dom)
{
}

const Function::Domain& Function::domain() const
{
	return domain_;
}

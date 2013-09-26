#include <util.h>

#include <exception>
#include <limits>

using std::exception;
using std::string;
using std::numeric_limits;

void ThrowIndexOutOfRange()
{
    throw "Index is out of range";
}

void ThrowMatrixIncorrectSize()
{
    throw "The matrix has incorrect size";
}

void ThrowIncorrectObjectCount()
{
    throw "Incorrect count of features rows, targets or weights";
}

void ThrowIncorrectFeatureCount()
{
    throw "Incorrect count of features";
}

double MissingValue()
{
    static double nan = numeric_limits<double>::infinity();
    return nan;
}

void ThrowIncorrectClassifierName()
{
    throw "Incorrect name classifier in Factory";
}

void ThrowIncorrectClassCount()
{    
	throw "Incorrect count of classes for classifier";
}

void ThrowIncorrectParametersString()
{    
	throw "Incorrect string of parameters";
}

void ThrowIncorrectTesterName()
{
    throw "Incorrect name tester in Factory";
}

bool isIntFromParametersString( const std::string& parameters, const std::string& nameParameter, int* value )
{
	//ToDo Реализовать функцию
	return false;
}

bool isDoubleFromParametersString( const std::string& parameters, const std::string& nameParameter, double* value )
{
	//ToDo Реализовать функцию
	return false;
}


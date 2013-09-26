#pragma once

#include <string>
// Helper

void ThrowIndexOutOfRange();
void ThrowMatrixIncorrectSize();
void ThrowIncorrectObjectCount();
void ThrowIncorrectFeatureCount();

// 'Constant' for missing values in learning data
double MissingValue();

//

void ThrowIncorrectClassifierName();

void ThrowIncorrectTesterName();

void ThrowIncorrectClassCount();

void ThrowIncorrectParametersString();

bool isIntFromParametersString( const std::string& parameters, const std::string& nameParameter, int* value );
bool isDoubleFromParametersString( const std::string& parameters, const std::string& nameParameter, double* value );



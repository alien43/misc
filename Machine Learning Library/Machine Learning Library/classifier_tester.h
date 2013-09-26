#pragma once

#include <vector>
#include <string>

#include <learning_data.h>

using std::vector;
using std::string;

class IClassifierFactory;

/*!
	\brief Base class for testers
	ClassifierTester should measure classifiers quality
*/
class ClassifierTester
{
public:
    virtual ~ClassifierTester()
    {
    }

    /*!
		\brief Get the classifier quality using the data
	*/
    virtual double Test(
        const IClassifierFactory& classifierFactory,
        const LearningData<int>& data) const = 0;
	/*!
		\brief ”станавливать параметры тестировани€
	*/
    virtual bool setParameters( const string& parameters )
	{
			return false;
	}

	/*!
		\brief ¬озвращает параметры тестировани€
	*/

	virtual void getParameters( string* parameters ) const
	{
		return parameters->clear();
	}
};

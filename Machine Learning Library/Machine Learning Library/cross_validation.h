#pragma once

#include <classifier_tester.h>

/** \file cross_validation.h
 * Cross-validation implementations of ClassifierTester:
 * QFoldTester (q-fold CV),
 * LeaveOneOutTester (leave-one-out CV),
 * RandomTester (random CV).
 */

/*!
	\brief q-fold cross-validation
*/
class QFoldTester: public ClassifierTester
{
private:
    /*!
		\brief q parameter of testing (number of folds)
	*/
    size_t q;

public:
    QFoldTester():
        q(10)
    {
    }

    /*!
		\brief q setter
	*/
    void SetFoldCount(size_t value)
    {
        if (value >= 2)
            q = value;
    }

    virtual double Test(
        const IClassifierFactory& classifierFactory,
        const LearningData<int>& data) const;
	/*!
		\brief ”станавливать параметры тестировани€
	*/
    virtual bool setParameters( const string& parameters );

	/*!
		\brief ¬озвращает параметры тестировани€
	*/
	virtual void getParameters( string* parameters );

};

/*!
	\brief Leave-one-out cross-validation
*/
class LeaveOneOutTester: public ClassifierTester
{
    virtual double Test(
        const IClassifierFactory& classifierFactory,
        const LearningData<int>& data) const;
};

/*!
	\brief Random cross-validation
*/
class RandomTester: public ClassifierTester
{
private:
    /*!
		\brief Number of times to test
	*/
    size_t testCount;

    /*!
		\brief Portion of objects for control data each time
	*/
    double testPortion;

public:
    RandomTester():
        testCount(10),
        testPortion(0.3)
    {
    }

    // Setters
    
    void SetTestCount(size_t value)
    {
        if (value >= 1)
            testCount = value;
    }

    void SetTestPortion(double value)
    {
        if (value > 1E-6 && value < 1 - 1E-6)
            testPortion = value;
    }
    
    virtual double Test(
        const IClassifierFactory& classifierFactory,
        const LearningData<int>& data) const;

	/*!
		\brief ”станавливать параметры тестировани€
	*/
    virtual bool setParameters( const string& parameters );

	/*!
		\brief ¬озвращает параметры тестировани€
	*/
	virtual void getParameters( string* parameters );
};

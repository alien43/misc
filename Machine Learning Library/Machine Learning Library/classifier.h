#pragma once

#include <vector>
#include <map>
#include <climits>

#include <learner.h>

using std::vector;
using std::map;

/*! \class Classifier classifier.h "classifier.h"
 *  \brief Base classifier class. Inherits Learner<int>.
 *
 * Stores vector of object classes and their indices in this vector.
 * Implements penalty function via the matrix.
 * Support special constant REFUSE to refuse from classification.
 * Estimates object to class outfit probability.
 */

class Classifier: public Learner<int>
{
public:
    /*!
	    \brief Special constant for refusal of classification
	*/
    static const int REFUSE;

private:
    /*!
	    \brief Vector of all classes of objects in appropriate data
	*/
    vector<int> classes;

    /*!
	    \brief Indices of classes in the classes vector
	*/
    map<int, size_t> classIndexes;

    /*!
	    \brief Penalty matrix (used in PredictionPenalty(...))
	*/
    Matrix penaltyMatrix;

public:
    Classifier(size_t featureCount, const vector<int>& classes);

    /*!
	    \brief Getters and setters
	*/
    double PenaltyMatrix(int actualClass, int predictedClass) const
    {
        return penaltyMatrix(
            classIndexes.find(actualClass)->second, 
            classIndexes.find(predictedClass)->second);
    }

    double& PenaltyMatrix(int actualClass, int predictedClass)
    {
        return penaltyMatrix(
            classIndexes.find(actualClass)->second,
            classIndexes.find(predictedClass)->second);
    }

    const map<int, size_t>& ClassIndexes() const
    {
        return classIndexes;
    }

    size_t ClassIndexes(int _class) const
    {
        return _class == REFUSE ? classes.size() : classIndexes.find(_class)->second;
    }

	/*!
	    \brief Returns vector of class values.
	*/
    const vector<int>& Classes() const
    {
        return classes;
    }

    int Classes(size_t index) const
    {
        return index < classes.size() ? classes[index] : REFUSE;
    }

	/*!
	    \brief Count of all classes that can be used in appropriate data.
	*/
    size_t ClassCount() const
    {
        return classes.size();
    }

	/*!
	    \brief Overrides Learner::PredictionPenalty.
	    Note: Learner::PenaltyFunc is useless in classifiers
	*/
    double PredictionPenalty(int actualTarget, int predictedTarget) const;

	/*!
	    \brief Get probability distribution for class of an object with the features.
	*/
    void PredictProbabilities(const vector<double>& features, vector<double>* probabilities) const;
  
  	/*!
	    \brief Get probability distribution for classes of objects with the features matrix.
	*/
    void PredictProbabilities(const Matrix& features, Matrix* probabilities) const;
    
protected:
	/*!
	    \brief Protected virtual version of the Predict(...) method
	    No need to check parameters here
	*/
    virtual void DoPredictProbabilities(const vector<double>& features, vector<double>* probabilities) const;
};

#pragma once

#include <vector>

#include <util.h>
#include <matrix.h>
#include <learning_data.h>

using std::vector;
using std::string;

/*!
	\brief Equality penalty function 
*/
template<class TTarget>
double SimplePenalty(TTarget actualTarget, TTarget predictedTarget);

/*!
	\brief Quadratic penalty function
*/
template<class TTarget>
double QuadraticPenalty(TTarget actualTarget, TTarget predictedTarget);

/*!
	\brief Base class for machine learning algorithms (classifiers, etc.)
	TTarget - type of targets (classes or reals)
*/
template<class TTarget>
class Learner
{
public:
    /*!
		\brief Count of features in appropriate learning data
	*/
    const size_t FeatureCount;

private:
    /*!
		\brief Penalty function (used in PredictionPenalty(...))
	*/
    double (*PenaltyFunc)(TTarget actualTarget, TTarget predictedTarget);

public:
    explicit Learner(size_t featureCount):
        FeatureCount(featureCount),
        PenaltyFunc(QuadraticPenalty)
    {
    }

    virtual ~Learner()
    {
    }

    /*!
		\brief PenaltyFunc setter
	*/
    void SetPenaltyFunc(double (*penaltyFunc)(TTarget actualTarget, TTarget predictedTarget))
    {
        if (penaltyFunc != NULL)
            PenaltyFunc = penaltyFunc;
    }

    /*!
		\brief Train learner
	*/
    void Train(const LearningData<TTarget>& data);

    /*!
		\brief Predict the target for an object with the features
	*/
    TTarget Predict(const vector<double>& features) const;

    /*!
		\brief Predict the targets for objects with the features matrix
	*/
    void Predict(const Matrix& features, vector<TTarget>* targets) const;

    /*!
		\brief Test learner with the control data
		Returns the quality of the learner
		ToDo: normalize?
	*/
    double Test(const LearningData<TTarget>& data) const;

    /*!
		\brief Penalty for incorrect prediction
	*/
    virtual double PredictionPenalty(TTarget actualTarget, TTarget predictedTarget) const;

    /*!
		\brief Get how valuable the feature is
	*/
    double GetFeatureWeight(size_t featureIndex) const;

	/*!
		\brief Устанавливать параметры обучающихся алгоритмов
	*/
	virtual bool setParameters( const string& parameters )
	{
		return false;	
	}
	/*!
		\brief Выдает параметры обучающихся алгоритмов
	*/
	virtual void getParameters( string* parameters ) const
	{
		parameters->clear();
	}

protected:
    /*!
		\brief Protected virtual versions of methods
		No need to check parameters in them
	*/
    virtual void DoTrain(const LearningData<TTarget>& data) = 0;

    virtual TTarget DoPredict(const vector<double>& features) const = 0;

    virtual double DoGetFeatureWeight(size_t featureIndex) const
    {
        return 1;
    }
};


// --- Implementation ---

template<class TTarget>
void Learner<TTarget>::Train(const LearningData<TTarget>& data)
{
    if (data.FeatureCount() != FeatureCount)
        ThrowIncorrectFeatureCount();
    DoTrain(data);
}

template<class TTarget>
TTarget Learner<TTarget>::Predict(const vector<double>& features) const
{
    if (features.size() != FeatureCount)
        ThrowIncorrectFeatureCount();
    return DoPredict(features);
}

template<class TTarget>
void Learner<TTarget>::Predict(const Matrix& features, vector<TTarget>* targets) const
{
    if (features.Width() != FeatureCount)
        ThrowIncorrectFeatureCount();
    if (targets->size() != features.Height())
        targets->resize(features.Height());
    for (size_t i = 0; i < features.Height(); ++i)
    {
        vector<double> row;
        features.GetRow(i, &row);
        (*targets)[i] = DoPredict(row);
    }
}

template<class TTarget>
double Learner<TTarget>::Test(const LearningData<TTarget>& data) const
{
    if (data.FeatureCount() != FeatureCount)
        ThrowIncorrectFeatureCount();
    vector<TTarget> predictedTargets(data.ObjectCount());
    Predict(data.Features(), &predictedTargets);
    double res = 0, weightSum = 0;
    for (size_t i = 0; i < data.ObjectCount(); ++i)
    {
        res += data.Weights(i) * PredictionPenalty(predictedTargets[i], data.Targets(i));
        weightSum += data.Weights(i);
    }
    return weightSum == 0 ? 1 : 1 - res / weightSum;
}

template<class TTarget>
double Learner<TTarget>::PredictionPenalty(
    TTarget predictedTarget,
    TTarget actualTarget) const
{
    return PenaltyFunc(actualTarget, predictedTarget);
}


template<class TTarget>
double Learner<TTarget>::GetFeatureWeight(size_t featureIndex) const
{
    if (featureIndex >= FeatureCount)
        return 0;
    return DoGetFeatureWeight(featureIndex);
}


// --- Standard penalty functions ---

template<class TTarget>
double SimplePenalty(TTarget actualTarget, TTarget predictedTarget)
{
    return predictedTarget == actualTarget ? 0 : 1;
}

template<class TTarget>
double QuadraticPenalty(TTarget actualTarget, TTarget predictedTarget)
{
    return (predictedTarget - actualTarget) * (predictedTarget - actualTarget);
}

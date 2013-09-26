#pragma once

#include <vector>
#include <set>
#include <fstream>

#include <util.h>
#include <matrix.h>

using std::vector;
using std::set;
using std::ifstream;
using std::ofstream;

/*!
	\brief Data for learning and predicting
	TTarget - type of targets (classes or reals)
*/
template<class TTarget>
class LearningData
{
private:
    /*!
		\brief Features-objects matrix
	*/
    Matrix features;

    /*!
		\brief Targets of objects
	*/
    vector<TTarget> targets;

    /*!
		\brief Weights of objects
	*/
    vector<double> weights;

public:
    LearningData()
    {
    }

    LearningData(const Matrix& features, const vector<TTarget>& targets, const vector<double>& weights):
        features(features),
        targets(targets),
        weights(weights)
    {
    }

    // Getters and setters

    const Matrix& Features() const
    {
        return features;
    }

    double Features(size_t objectIndex, size_t featureIndex) const
    {
        return features(objectIndex, featureIndex);
    }

    double& Features(size_t objectIndex, size_t featureIndex)
    {
        return features(objectIndex, featureIndex);
    }

    void GetFeatures(size_t objectIndex, vector<double>* outFeatures) const
    {
        return Features().GetRow(objectIndex, outFeatures);
    }

    const vector<TTarget>& Targets() const
    {
        return targets;
    }

    TTarget Targets(size_t index) const
    {
        return targets[index];
    }

    TTarget& Targets(size_t index)
    {
        return targets[index];
    }

    const vector<double>& Weights() const
    {
        return weights;
    }

    double Weights(size_t index) const
    {
        return weights[index];
    }

    double& Weights(size_t index)
    {
        return weights[index];
    }

    /*!
		\brief Count of objects in data
	*/
    size_t ObjectCount() const
    {
        return targets.size();
    }

    /*!
		\brief Count of objects features
	*/
    size_t FeatureCount() const
    {
        return features.Width();
    }

    /*!
		\brief Append more objects
	*/
    void AppendData(const LearningData<TTarget>& data);

    /*!
		\brief Append the object
	*/
    void AppendObject(const vector<double>& featuresRow, TTarget target, double weight = 1);

    /*!
		\brief Append the feature values to all objects in data
	*/
    void AppendFeature(const vector<double>& featureValues);

    /*!
		\brief Select several objects in data
	*/
    template<class TIterator>
    void SelectObjects(TIterator firstIndex, TIterator lastIndex, LearningData<TTarget>* outData) const;

    /*!
		\brief Select several features in data
	*/
    template<class TIterator>
    void SelectFeatures(TIterator firstIndex, TIterator lastIndex, LearningData<TTarget>* outData) const;

    /*!
		\brief Select several objects and features
	*/
    template<class TIterator1, class TIterator2>
    void SelectObjectsAndFeatures(
        TIterator1 firstObjectIndex, TIterator1 lastObjectIndex,
        TIterator2 firstFeatureIndex, TIterator2 lastFeatureIndex,
        LearningData<TTarget>* outData) const;

    /*!
		\brief Save data to file
	*/
    void Save(const char* fileName) const
    {
        ofstream output(fileName);
        Save(output);
    }

    /*!
		\brief Load data from file
	*/
    void Load(const char* fileName)
    {
        ifstream input(fileName);
        Load(input);
    }

    /*!
		\brief Save data to stream
	*/
    void Save(ostream& output) const;

    /*!
		\brief Load data from stream
	*/
    void Load(istream& input);
};


/*!
	\brief Get vector of unique classes in the data
*/
inline vector<int> GetClasses(const LearningData<int>& data)
{
    set<int> unique;
    unique.insert(data.Targets().begin(), data.Targets().end());
    return vector<int>(unique.begin(), unique.end());
}


// --- Implementation ---

template<class TTarget>
template<class TIterator>
void LearningData<TTarget>::SelectObjects(TIterator firstIndex, TIterator lastIndex, LearningData<TTarget>* outData) const
{
    size_t size = lastIndex - firstIndex;
    outData->targets.resize(size);
    outData->weights.resize(size);
    size_t i = 0;
    for (TIterator it = firstIndex; it != lastIndex; ++it, ++i)
    {
        outData->targets[i] = targets[*it];
        outData->weights[i] = weights[*it];
    }
    features.SelectRows(firstIndex, lastIndex, &(outData->features));
}

template<class TTarget>
template<class TIterator>
void LearningData<TTarget>::SelectFeatures(TIterator firstIndex, TIterator lastIndex, LearningData<TTarget>* outData) const
{
    features.SelectColumns(firstIndex, lastIndex, &(outData->features));
}

template<class TTarget>
template<class TIterator1, class TIterator2>
void LearningData<TTarget>::SelectObjectsAndFeatures(
    TIterator1 firstObjectIndex, TIterator1 lastObjectIndex, 
    TIterator2 firstFeatureIndex, TIterator2 lastFeatureIndex,
    LearningData<TTarget>* outData) const
{
    size_t size = lastObjectIndex - firstObjectIndex;
    outData->targets.resize(size);
    outData->weights.resize(size);
    size_t i = 0;
    for (TIterator1 it = firstObjectIndex; it != lastObjectIndex; ++it, ++i)
    {
        outData->targets[i] = targets[*it];
        outData->weights[i] = weights[*it];
    }
    features.GetMinor(
        firstObjectIndex, lastObjectIndex,
        firstFeatureIndex, lastFeatureIndex,
        outData->features);
}

template<class TTarget>
void LearningData<TTarget>::AppendData(const LearningData<TTarget>& data)
{
    if (ObjectCount() == 0)
    {
        *this = data;
        return;
    }
    if (data.ObjectCount() == 0)
        return;
    if (data.FeatureCount() != FeatureCount())
        ThrowIncorrectFeatureCount();

    features.AppendDown(data.features);
    targets.insert(targets.end(), data.Targets().begin(), data.Targets().end());
    weights.insert(weights.end(), data.Weights().begin(), data.Weights().end());
}

template<class TTarget>
void LearningData<TTarget>::AppendObject(
    const vector<double>& featuresRow, 
    TTarget target,
    double weight /* = 1*/)
{
    if (ObjectCount() != 0 && featuresRow.size() != FeatureCount())
        ThrowIncorrectFeatureCount();
    features.AppendRow(featuresRow);
    targets.push_back(target);
    weights.push_back(weight);
}

template<class TTarget>
void LearningData<TTarget>::AppendFeature(const vector<double>& featureValues)
{
    if (featureValues.size() != ObjectCount())
        ThrowIncorrectObjectCount();
    features.AppendVector(featureValues);
}

template<class TTarget>
void LearningData<TTarget>::Save(ostream& output) const
{
    output << FeatureCount() << "\t" << ObjectCount() << std::endl;
    for (size_t i = 0; i < ObjectCount(); ++i)
    {
        output << Weights(i) << "\t" << Targets(i);
        for (size_t j = 0; j < FeatureCount(); ++j)
            output << "\t" << Features(i, j);
        output << std::endl;
    }
}

template<class TTarget>
void LearningData<TTarget>::Load(istream& input)
{
    size_t featureCount, objectCount;
    input >> featureCount >> objectCount;
    features.Resize(objectCount, featureCount);
    targets.resize(objectCount);
    weights.resize(objectCount);
    for (size_t i = 0; i < objectCount; ++i)
    {
        input >> weights[i] >> targets[i];
        for (size_t j = 0; j < featureCount; ++j)
            input >> features(i, j);
    }
}

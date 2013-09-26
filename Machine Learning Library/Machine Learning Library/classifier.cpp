#include <classifier.h>

const int Classifier::REFUSE = INT_MIN;

Classifier::Classifier(size_t featureCount, const vector<int>& classes):
    Learner<int>(featureCount),
    classes(classes),
    classIndexes(),
    penaltyMatrix(classes.size(), classes.size() + 1, 1)
{
    for (size_t i = classes.size(); i > 0; --i)
        classIndexes[classes[i - 1]] = i - 1;
    for (size_t i = 0; i < penaltyMatrix.Height(); ++i)
        penaltyMatrix(i, i) = 0;
}

double Classifier::PredictionPenalty(int actualTarget, int predictedTarget) const
{
    return PenaltyMatrix(actualTarget, predictedTarget);
}

void Classifier::PredictProbabilities(const vector<double>& features, vector<double>* probabilities) const
{
    if (features.size() != FeatureCount)
        ThrowIncorrectFeatureCount();
    if (probabilities->size() != ClassCount())
        probabilities->resize(ClassCount());
    DoPredictProbabilities(features, probabilities);
}

void Classifier::PredictProbabilities(const Matrix& features, Matrix* probabilities) const
{
    if (features.Width() != FeatureCount)
        ThrowIncorrectFeatureCount();
    probabilities->Resize(0, 0);
    for (size_t i = 0; i < features.Height(); ++i)
    {
        vector<double> probabilitiesRow, featuresRow;
        features.GetRow(i, &featuresRow);
        DoPredictProbabilities(featuresRow, &probabilitiesRow);
        probabilities->AppendRow(probabilitiesRow);
    }
}

void Classifier::DoPredictProbabilities(const vector<double>& features, vector<double>* probabilities) const
{
    for (size_t i = 0; i < probabilities->size(); ++i)
        (*probabilities)[i] = 0;
    int prediction = DoPredict(features);
    (*probabilities)[classIndexes.find(prediction)->second] = 1;
}

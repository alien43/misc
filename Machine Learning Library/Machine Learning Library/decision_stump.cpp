#include "decision_stump.h"
#include <algorithm>

using std::vector;
using std::sort;
using std::unique;

DecisionStump::DecisionStump(size_t featureCount, const vector<int>& classes) : 
	Classifier(featureCount, classes)
	{}

void DecisionStump::DoTrain(const LearningData<int>& data) {
	DecisionType bestDecisionType = DT_LESS;
	double bestValue = 0.0;
	size_t bestIFeature = 0;
	double bestQuality = -1.0;
	for (int iDT = 0; iDT <= 1; ++iDT) {
		decisionType_ = DecisionType(iDT);
		for (iFeature_ = 0; iFeature_ < data.FeatureCount(); ++iFeature_) {
			vector<double> allValues;
			for (size_t i = 0; i < data.ObjectCount(); ++i) {
				allValues.push_back(data.Features(i, iFeature_));
			}
			sort(allValues.begin(), allValues.end());
			allValues.resize(unique(allValues.begin(), allValues.end()) - 
				allValues.begin());
			for (size_t iValue = 0; iValue < allValues.size(); ++iValue) {
				value_ = allValues[iValue];
				const double quality = GetCurrentQuality(data);
				if (bestQuality < quality) {
					bestDecisionType = decisionType_;
					bestIFeature = iFeature_;
					bestValue = value_;
					bestQuality = quality;
				}
			}
		}
	}
	decisionType_ = bestDecisionType;
	iFeature_ = bestIFeature;
	value_ = bestValue;
}

int DecisionStump::DoPredict(const vector<double>& features) {
	return (decisionType_ == DT_LESS && features.at(iFeature_) < value_
		||  decisionType_ == DT_MORE && features.at(iFeature_) > value_);
}

double DecisionStump::GetCurrentQuality(const LearningData<int>& data) const {
	const size_t nObjectsTotal = data.ObjectCount();
	if (nObjectsTotal == 0) {
		return 0.0;
	}

	size_t nObjectsRight = 0;
	for (size_t i = 0; i < nObjectsTotal; ++i) {
		vector<double> features;
		data.GetFeatures(i, &features);
		// DoPredict is non-const for some reason, that is why
		// const_cast is used.
		const int predictionResult = 
			const_cast<DecisionStump*>(this)->DoPredict(features);
		const int rightResult = data.Targets(i);
		const bool isResultRight = (predictionResult == rightResult);
		if (isResultRight) {
			++nObjectsRight;
		}
	}

	const double quality = (double)nObjectsRight / nObjectsTotal;
	return quality;
}

void DecisionStump::Clear() {
}

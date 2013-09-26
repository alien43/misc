#ifndef DECISION_STUMP_H_INCLUDED
#define DECISION_STUMP_H_INCLUDED

#include "decision_rule_classifier.h"

#include <vector>

class DecisionStump : public Classifier {
public:
	enum DecisionType {
		DT_LESS = 0, DT_MORE = 1
	};

	DecisionStump(size_t featureCount, const vector<int>& classes);
	void Clear();

protected:
    void DoTrain(const LearningData<int>& data);
	int DoPredict(const vector<double>& features);

private:
	double GetCurrentQuality(const LearningData<int>& data) const;

	size_t iFeature_;
	DecisionType decisionType_;
	double value_;
};

#endif //DECISION_STUMP_H_INCLUDED
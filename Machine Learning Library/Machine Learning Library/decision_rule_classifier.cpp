#include <decision_rule_classifier.h>

DecisionRuleClassifier::DecisionRuleClassifier(size_t featureCount, const vector<int>& classes):
    Classifier(featureCount, classes),
    DecisionRule(ArgMax)
{
}

void DecisionRuleClassifier::GetEstimates(const vector<double>& features, vector<double>* outEstimates) const
{
    if (features.size() != FeatureCount)
        ThrowIncorrectFeatureCount();
    outEstimates->assign(ClassCount(), 0);
    DoGetEstimates(features, outEstimates);
}

int DecisionRuleClassifier::DoPredict(const vector<double>& features) const
{
    vector<double> estimates;
    GetEstimates(features, &estimates);
    return Classes(DecisionRule(estimates));
}

size_t ArgMax(const vector<double>& estimates)
{
    size_t argMax = estimates.size();
    for (size_t i = 0; i < estimates.size(); ++i)
    {
        if (argMax == estimates.size() || estimates[i] > estimates[argMax])
            argMax = i;
    }
    return argMax;
}
#pragma once

#include <vector>

#include <classifier.h>

using std::vector;

/*!
	\brief Base class for classifiers using decision rule
*/
class DecisionRuleClassifier: public virtual Classifier
{
private:
    /*!
		\brief Decision rule used for the final prediction
	*/
    size_t (*DecisionRule)(const vector<double>& estimates);

public:
    DecisionRuleClassifier(size_t featureCount, const vector<int>& classes);

    virtual ~DecisionRuleClassifier()
    {
    }

    /*!
		\brief DesicionRule setter
	*/
    void SetDecisionRule(size_t (*decisionRule)(const vector<double>& estimates))
    {
        if (decisionRule != NULL)
            DecisionRule = decisionRule;
    }

    /*!
		\brief Get estimate values for each class for an object with the features
		These estimates are used in decision rule for the final prediction
	*/
    void GetEstimates(const vector<double>& features, vector<double>* outEstimates) const;

protected:
    /*!
		\brief Learner::DoPredict
	*/
    virtual int DoPredict(const vector<double>& features) const;

    /*!
		\brief Protected virtual version of the GetEstimates(...) method
		No need to check parameters here
	*/
    virtual void DoGetEstimates(
        const vector<double>& features,
        vector<double>* outEstimates) const = 0;
};


// --- Decision rules ---

/*!
	\brief Get class with the maximal estimate
*/
size_t ArgMax(const vector<double>& estimates);
#include <stdio.h>
#include <iostream>

#include "util.h"
#include "matrix.h"
#include "tester.h"
#include "classifier.h"
#include "cross_validation.h"
#include "decision_stump.h"

using std::cout;
using std::endl;

int main() {
    // Loading data from file
    cout << "Loading 'Iris' data..." << endl;
    LearningData<int> data;
    data.Load("iris2simple.txt");
    cout << "Loaded " << data.ObjectCount() << " objects with " << 
                 data.FeatureCount() << " features." << endl;

    cout << "Testing single decision stump classifier..." << endl;

    // Creating classifier for the data
    Classifier* classifier = new DecisionStump(data.FeatureCount(), GetClasses(data));

    // Creating different testers and testing the classifier
    Tester<int>* tester;
    tester = new QFoldTester<int>();
    cout << "q-fold test: " << tester->Test(classifier, data) << endl;
    delete tester;
/*
    tester = new LeaveOneOutTester<int>();
    cout << "Leave-one-out test: " << tester->Test(classifier, data) << endl;
    delete tester;
*/
    tester = new RandomTester<int>();
    cout << "Random test: " << tester->Test(classifier, data) << endl;
    delete tester;

    delete classifier;

	return 0;
}

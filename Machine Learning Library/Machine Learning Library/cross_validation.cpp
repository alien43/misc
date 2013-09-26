#include <cross_validation.h>

#include <algorithm>
#include <vector>
#include <sstream>

#include <factories.h>
#include <learning_data.h>
#include <factories_tester.h>

using std::min;
using std::vector;
using std::stringstream;

static const bool REGISTERED_QFOLD =
    ClassifierTestersFactory::Instance().RegisterClassifierTesterFactory(
        "Tester.TesterClassifier.QFoldTester",
        new DefaultClassifierTesterFactory<QFoldTester>,
		"Q fold tester");

static const bool REGISTERED_LEAVE_ONE_OUT =
    ClassifierTestersFactory::Instance().RegisterClassifierTesterFactory(
        "Tester.TesterClassifier.LeaveOneOutTester",
        new DefaultClassifierTesterFactory<LeaveOneOutTester>,
		"Leave one out tester");

static const bool REGISTERED_RANDOM =
    ClassifierTestersFactory::Instance().RegisterClassifierTesterFactory(
        "Tester.TesterClassifier.RandomTester",
        new DefaultClassifierTesterFactory<RandomTester>,
		"Random tester");

double TestClassifier(
    const IClassifierFactory& classifierFactory,
    const vector<int>& classes,
    const LearningData<int>& trainData,
    const LearningData<int>& testData)
{
    Classifier* classifier = 
        classifierFactory.CreateClassifier(
        trainData.FeatureCount(),
        classes);
    classifier->Train(trainData);
    return classifier->Test(testData);
}

double QFoldTester::Test(
    const IClassifierFactory& classifierFactory,
    const LearningData<int>& data) const
{
    double quality = 0;
    size_t count = 0;

    size_t length = data.ObjectCount();
    size_t foldLength = (length + q - 1) / q;

    vector<size_t> indexes(length);
    for (size_t i = 0; i < length; ++i)
        indexes[i] = i;

    for (size_t i = 0; i < length; i += foldLength)
    {
        vector<size_t>::iterator itStart = indexes.begin() + i;
        vector<size_t>::iterator itEnd = indexes.begin() + min(i + foldLength, length);
        LearningData<int> testData, trainData, trainData2;
        data.SelectObjects(itStart, itEnd, &testData);
        data.SelectObjects(indexes.begin(), itStart, &trainData);
        data.SelectObjects(itEnd, indexes.end(), &trainData2);
        trainData.AppendData(trainData2); // ToDo: optimize

        if (trainData.ObjectCount() == 0 || testData.ObjectCount() == 0)
            continue;

        quality += TestClassifier(classifierFactory, GetClasses(data), trainData, testData);
        ++count;
    }

    return count == 0 ? 0 : quality / count;
}

bool QFoldTester::setParameters( const string& parameters )
{
	int qTemp = 1;
	if( isIntFromParametersString( parameters, "Q", &qTemp ) ){
		if( qTemp <= 1 )
			ThrowIncorrectParametersString();
		else {
			q = qTemp;
			 
		}
	} else {
		return false;
	}
	return true;
}

void QFoldTester::getParameters( string* parameters )
{	
	stringstream stream;
	stream << "Q = " << q;
	parameters->clear();
	parameters->append( stream.str() );
}

double LeaveOneOutTester::Test(const IClassifierFactory& classifierFactory, const LearningData<int>& data ) const
{
    size_t length = data.ObjectCount();
    vector<size_t> indexes(length);
    for (size_t i = 0; i < length; ++i)
        indexes[i] = i;

    double quality = 0;

    LearningData<int> trainData;
    data.SelectObjects(indexes.begin() + 1, indexes.end(), &trainData);
    for (size_t i = 0; i < length; ++i)
    {
        if (i > 0)
        {
            for (size_t j = 0; j < data.FeatureCount(); ++j)
                trainData.Features(i - 1, j) = data.Features(i - 1, j);
            trainData.Targets(i - 1) = data.Targets(i - 1);
            trainData.Weights(i - 1) = data.Weights(i - 1);
        }
        LearningData<int> testData;
        data.SelectObjects(&i, &i + 1, &testData);

        quality += TestClassifier(classifierFactory, GetClasses(data), trainData, testData);
    }
    return length == 0 ? 0 : quality / length;
}

double RandomTester::Test( const IClassifierFactory& classifierFactory, const LearningData<int>& data ) const
{
    size_t length = data.ObjectCount();
    double quality = 0;

    for (size_t i = 0; i < testCount; ++i)
    {
        vector<bool> forTest(length);
        for (size_t j = 0; j < testPortion * length; ++j)
        {
            size_t index;
            do
            {
                index = rand() % length;				
            }
            while (forTest[index]);
            forTest[index] = true;
        }

        vector<size_t> trainIndexes, testIndexes;
        for (size_t j = 0; j < length; ++j)
            (forTest[j] ? testIndexes : trainIndexes).push_back(j);
        LearningData<int> trainData, testData;
        data.SelectObjects(trainIndexes.begin(), trainIndexes.end(), &trainData);
        data.SelectObjects(testIndexes.begin(), testIndexes.end(), &testData);

        quality += TestClassifier(classifierFactory, GetClasses(data), trainData, testData);
    }

    return quality / testCount;
}

bool RandomTester::setParameters( const string& parameters )
{
	int Count = 1;
	if( isIntFromParametersString( parameters, "Count", &Count ) ){
		if( Count <= 1 )
			ThrowIncorrectParametersString();
		else {
			testCount = Count;
		}
	} else {
		return false;
	}
	double Portion = 0.;
	if( isDoubleFromParametersString( parameters, "Portion", &Portion ) ){
		if( Count >= 0. )
			ThrowIncorrectParametersString();
		else {
			testPortion = Portion;
		}
	} else {
		return false;
	}
	return true;
}

void RandomTester::getParameters( string* parameters )
{	
	stringstream stream;
	stream << "Count = " << testCount << " Portion = " << testPortion;
	parameters->clear();	
	parameters->append( stream.str() );
}
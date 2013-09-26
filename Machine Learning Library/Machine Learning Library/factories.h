#pragma once

#include <map>
#include <string>
#include <set>

#include <util.h>
#include <classifier.h>

using std::map;
using std::string;
using std::pair;
using std::set;

class IClassifierFactory
{
public:
    virtual Classifier* CreateClassifier(
        size_t featureCount,
        const vector<int>& classes) const = 0;
};

template<class TClassifier>
class DefaultClassifierFactory: public IClassifierFactory
{
public:
    virtual Classifier* CreateClassifier(
        size_t featureCount,
        const vector<int>& classes) const
    {
        return new TClassifier(featureCount, classes);
    }
};

/**
* ���������� ���������������, ���������
*/
class MachineLearningFactory
{
private:
    typedef pair<const IClassifierFactory*, string> ClassifierFactoryEntry;

private:
    map<string, ClassifierFactoryEntry> classifierFactories;

private:
	MachineLearningFactory();
	MachineLearningFactory(MachineLearningFactory& factory);
	~MachineLearningFactory();

public:
	///���������� ������������ ������
	static MachineLearningFactory& Instance();

	///������������ �������������
	bool RegisterClassifierFactory(
        const string& classifierName,
        const IClassifierFactory* classifierFactory,
        const string& classifierAnnotation);

	///������� ������������� �� ����� � ������� ��������� new
	Classifier* CreateClassifier(
        const string& classifierName,
        size_t featureCount,
        const vector<int>& classes);

    const IClassifierFactory* GetClassifierFactory(const string& classifierName);

	void GetClassifierNames(set<string>* outNames);

	const string& GetClassifierAnnotation(const string& classifierName);
};



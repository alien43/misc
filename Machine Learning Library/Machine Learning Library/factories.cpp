#include <factories.h>

using std::map;
using std::string;


MachineLearningFactory::MachineLearningFactory()
{
}

MachineLearningFactory::MachineLearningFactory( MachineLearningFactory& factory )
{
}

MachineLearningFactory::~MachineLearningFactory()
{
}

MachineLearningFactory& MachineLearningFactory::Instance()
{
	static MachineLearningFactory factory;
	return factory;
}

bool MachineLearningFactory::RegisterClassifierFactory(
    const string& classifierName,
    const IClassifierFactory* classifierFactory,
    const string& classifierAnnotation)
{
    if (classifierFactories.find(classifierName) == classifierFactories.end())
    {
        classifierFactories.insert(
            std::make_pair(classifierName,
                std::make_pair(classifierFactory, classifierAnnotation)));
        return true;
    }
    else
    {
        return false;
    }
}


const IClassifierFactory* MachineLearningFactory::GetClassifierFactory(
    const string& classifierName)
{
    map<string, ClassifierFactoryEntry>::iterator it =
        classifierFactories.find(classifierName);
    if(it == classifierFactories.end()) 
    {	
        ThrowIncorrectClassifierName();
    }
    return it->second.first;
}

Classifier* MachineLearningFactory::CreateClassifier(
    const string& classifierName,
    size_t featureCount, 
    const vector<int>& classes)
{
    return GetClassifierFactory(classifierName)->CreateClassifier(featureCount, classes);
}

void MachineLearningFactory::GetClassifierNames(std::set<std::string>* outNames)
{
	outNames->clear();
	for(map<string, ClassifierFactoryEntry>::const_iterator it = classifierFactories.begin();
        it != classifierFactories.end();
        ++it)
    {
		outNames->insert(it->first);
    }
}

const string& MachineLearningFactory::GetClassifierAnnotation(const string& classifierName)
{
    map<string, ClassifierFactoryEntry>::const_iterator it =
        classifierFactories.find(classifierName);
    if(it == classifierFactories.end()) 
    {	
        ThrowIncorrectClassifierName();
    }
    return it->second.second;
}

#include <factories_tester.h>

using std::map;
using std::string;


ClassifierTestersFactory::ClassifierTestersFactory()
{
}

ClassifierTestersFactory::ClassifierTestersFactory( ClassifierTestersFactory& factory )
{
}

ClassifierTestersFactory::~ClassifierTestersFactory()
{
}

ClassifierTestersFactory& ClassifierTestersFactory::Instance()
{
	static ClassifierTestersFactory factory;
	return factory;
}

bool ClassifierTestersFactory::RegisterClassifierTesterFactory(
        const string& testerName,
        const ITesterFactory* testerFactory,
        const string& testerAnnotation)
{
    if( testerFactories.find( testerName ) == testerFactories.end())
    {
        testerFactories.insert(
            std::make_pair(testerName,
                std::make_pair(testerFactory, testerAnnotation)));
        return true;
    }
    else
    {
        return false;
    }
}


const ITesterFactory* ClassifierTestersFactory::GetClassifierTesterFactory(
    const string& classifierName)
{
    map<string, ClassifierTesterFactoryEntry>::iterator it =
        testerFactories.find(classifierName);
    if(it == testerFactories.end()) 
    {	
        ThrowIncorrectTesterName();
    }
    return it->second.first;
}

ClassifierTester* ClassifierTestersFactory::CreateClassifierTester(
        const string& testerName )
{
    return GetClassifierTesterFactory(testerName)->
		CreateClassifierTester();
}

void ClassifierTestersFactory::GetTesterNames(set<string>* outNames)
{
	outNames->clear();
	for(map<string, ClassifierTesterFactoryEntry>::const_iterator it = 
		testerFactories.begin();
        it != testerFactories.end();
        ++it)
    {
		outNames->insert(it->first);
    }
}

const string& ClassifierTestersFactory::GetTesterAnnotation(const string& testerName)
{
    map<string, ClassifierTesterFactoryEntry>::const_iterator it =
        testerFactories.find(testerName);
    if(it == testerFactories.end()) 
    {	
        ThrowIncorrectTesterName();
    }
    return it->second.second;
}

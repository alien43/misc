#pragma once

#include <map>
#include <string>
#include <set>

#include <util.h>
#include <classifier_tester.h>

using std::map;
using std::string;
using std::pair;
using std::set;

class ITesterFactory
{
public:
    virtual ClassifierTester* CreateClassifierTester() const = 0;
};

template<class TTester>
class DefaultClassifierTesterFactory: public ITesterFactory
{
public:
    virtual ClassifierTester* CreateClassifierTester() const
    {
        return new TTester();
    }
};

/**
* Библиотека классификаторов, синглетон
*/
class ClassifierTestersFactory
{
private:
    typedef pair<const ITesterFactory*, string> ClassifierTesterFactoryEntry;

private:
    map<string, ClassifierTesterFactoryEntry> testerFactories;

private:
	ClassifierTestersFactory();
	ClassifierTestersFactory(ClassifierTestersFactory& factory);
	~ClassifierTestersFactory();

public:
	///Возвращает единственный объект
	static ClassifierTestersFactory& Instance();

	///Регистрирует скассификатор
	bool RegisterClassifierTesterFactory(
        const string& testerName,
        const ITesterFactory* testerFactory,
        const string& testerAnnotation);

	///Создает классификатор по имени с помощью оператора new
	ClassifierTester* CreateClassifierTester(
        const string& testerName );

    const ITesterFactory* GetClassifierTesterFactory(const string& testerName);

	void GetTesterNames(set<string>* outNames);

	const string& GetTesterAnnotation(const string& testerName);
};

#include <exception>
#include <stdio.h>
#include <iostream>

#include "../include/sqlite3.h"

#include "../include/util.h"
#include "../include/matrix.h"
#include "../include/tester.h"
#include "../include/classifier.h"
#include "../include/cross_validation.h"
#include "../include/classifier_factory.h"


using std::cout;
using std::endl;
using std::set;
using std::string;
/**
* Тестирует все слассификаторы создает табличку и записывает ее в wikiTrac
*/

class RowTableTesting{
public:
	std::string nameClassifier;
	std::string noteClassifier;
	double estimateQFoldTester;
	double estimateLeaveOneOutTester;
	double estimateRandomTester;
	std::string ToString() const{
		char text[1000];
		sprintf_s(text, "||%s||%s||%lf||%lf||%lf||",
			nameClassifier.c_str(), noteClassifier.c_str(),
			estimateQFoldTester, estimateLeaveOneOutTester, 
			estimateRandomTester );
		return text;
	}
};

void Estimate( const Tester<int>& qFoldTester, 
			  const Tester<int>& leaveOneOutTester,
			  const Tester<int>& randomTester,
			  const LearningData<int>& data,
			  RowTableTesting* estimates )
{
	Classifier* classifier = ClassifierFactory::Instance().CreateClassifier(
		estimates->nameClassifier.c_str(), data.FeatureCount(), GetClasses(data));
	estimates->noteClassifier = ClassifierFactory::Instance().getNoteAboutClassifier(
		estimates->nameClassifier.c_str() );
	estimates->estimateQFoldTester = qFoldTester.Test(classifier, data);
	estimates->estimateLeaveOneOutTester = leaveOneOutTester.Test(classifier, data);
	estimates->estimateRandomTester = randomTester.Test(classifier, data);
}

/**
*  Структура таблицы wiki в траке
*  CREATE TABLE wiki (
*      name text,
*      version integer,
*      time integer,
*      author text,
*      ipnr text,
*      text text,
*      comment text,
*      readonly integer,
*      UNIQUE (name,version)
*   );
*   CREATE INDEX wiki_time_idx ON wiki ( time);
*/

void writeToWikiTrac( const std::vector<RowTableTesting>& tableTesting, 
					 const std::string& namePage,
					 const std::string& dbName)
{
	sqlite3 *db;
	char* error_messages = 0;
	int return_value;
	return_value = sqlite3_open(dbName.c_str(), &db);
	if( return_value ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return;
	}
	std::string sqlText=
			"SELECT max(version) FROM wiki WHERE name='" + namePage + "'";
	const char * sqlTextChar = sqlText.c_str();
	sqlite3_stmt *ppVm;
	return_value = sqlite3_prepare(db, sqlTextChar, -1, &ppVm, &sqlTextChar );
	int version = 1;
	if( sqlite3_step(ppVm) != SQLITE_DONE )
	{
		version = sqlite3_column_int( ppVm, 0) + 1;
	}
	char version_str[100];
	sprintf_s( version_str, "%d", version );
    std::string Text="= Список классификаторов = \n\r";
	for( std::vector<RowTableTesting>::const_iterator  iclassifier = tableTesting.begin();
		iclassifier != tableTesting.end(); ++ iclassifier )
	{
		Text += iclassifier->ToString() + "\n\r";	
	}
	cout<<Text<<endl;
	sqlText  = "INSERT INTO wiki (name, version, time, author, text, ipnr, readonly, comment) \
			   VALUES ('"  + namePage +"', '"+version_str+"', '00:00:00', 'ashikhmin', '"+Text+"', '127.0.0.1', 0, '')";
	return_value=sqlite3_exec(db, sqlText.c_str(), NULL, NULL, &error_messages);
	return_value=sqlite3_exec(db, "commit;", NULL, NULL, &error_messages);
	return_value=sqlite3_finalize(ppVm);
	sqlite3_close(db);       
}

int main (int argc, char *argv[])
{
	if( argc != 4 )
	{	
		/*
		*   name_wiki_page = "test_all"
		*/
		cout << endl<< " programm name_wiki_page file_data db_name"  << endl;
		cout << endl<< "list classifiers:"  << endl;
		return 1;
	}
	/**
	* Загрузка данных 
	*/
	cout << "Loading 'Iris' data..." << endl;
	LearningData<int> data;
	data.Load( argv[2] );
	/**
	* Создание тестеров
	*/
	Tester<int>* qFoldTester =  new QFoldTester<int>();
	Tester<int>* leaveOneOutTester = new LeaveOneOutTester<int>();
	Tester<int>* randomTester = new RandomTester<int>(); 

	std::vector<RowTableTesting> tableTesting;
	RowTableTesting currentRowTableTesting;
	set<string> names;
	ClassifierFactory::Instance().getClassifierNames( &names );
	for( std::set<string>::iterator it_names = names.begin(); it_names != names.end(); ++ it_names )
	{	
		try {
			currentRowTableTesting.nameClassifier = *it_names;
			Estimate( *qFoldTester, *leaveOneOutTester, *randomTester, data,
				&currentRowTableTesting ); 
		} catch( std::exception& ex ){
			cout << ex.what();
			continue;
		}
		tableTesting.push_back(currentRowTableTesting);
	}
	/**
	* Удаление тестеров
	*/
	delete qFoldTester;
	delete leaveOneOutTester;
	delete randomTester;
	writeToWikiTrac( tableTesting, argv[1], argv[3] );
	return 0;
}
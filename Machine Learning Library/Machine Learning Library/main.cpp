#include <stdio.h>
#include <iostream>

#include <util.h>
#include <matrix.h>
#include <classifier_tester.h>
#include <classifier.h>
#include <cross_validation.h>
#include <factories.h>
#include <factories_tester.h>

using std::cout;
using std::endl;
using std::set;
using std::string;

/**
* первый передаваемый аргумент это имя классификатора 
* второй путь к файлу данных
*/
int main (int argc, char *argv[])
{
	if( argc != 4 )
	{	
		cout << endl<< "input parameters: classified tester file_data"  << endl<< endl;
		cout << endl<< "list classifiers:"  << endl;
		set<string> names;
		MachineLearningFactory::Instance().GetClassifierNames(&names);
		for( std::set<string>::iterator it_names = names.begin(); it_names != names.end(); ++ it_names )
			cout <<"        "<< *it_names  << endl;
		cout << endl<< "list testers:"  << endl;
		ClassifierTestersFactory::Instance().GetTesterNames(&names);
		for( std::set<string>::iterator it_names = names.begin(); it_names != names.end(); ++ it_names )
			cout <<"        "<< *it_names  << endl;
	} else {
		// Loading data from file
		cout << "Loading 'Iris' data..." << endl;
		LearningData<int> data;
		data.Load( argv[3] );
		cout << "Loaded " << data.ObjectCount() << " objects with " << 
                 data.FeatureCount() << " features." << endl;

		cout << "Tester "<<argv[2]<<" " << endl;
		cout <<  ClassifierTestersFactory::Instance().GetTesterAnnotation( argv[2] )<< endl;

		cout << "Classifier "<<argv[1]<<" " << endl;
		cout <<  MachineLearningFactory::Instance().GetClassifierAnnotation( argv[1] )<< endl;

		const IClassifierFactory* classifierFactory = 
            MachineLearningFactory::Instance().GetClassifierFactory(argv[1]);
        
		ClassifierTester* tester = ClassifierTestersFactory::Instance().
				CreateClassifierTester(argv[2]);
		cout << "Test: " << tester->Test(*classifierFactory, data) << endl;
		delete tester;
	}
}

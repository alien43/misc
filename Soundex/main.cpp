#include "soundex.h"

#include <iostream>
#include <fstream>
#include <string>


int main()
{

	std::ifstream inputFile("words.txt");
	std::string s1, s2;
	inputFile >> s1 >> s2;
	inputFile.close();

	SoundexCoder coder(3);
	std::ofstream outputFile("out.txt");
	outputFile << coder.code(s1) << std::endl << coder.code(s2) << std::endl;
	outputFile.close();
	std::cout << coder.doSoundSimilarly(s1, s2) << std::endl;
}

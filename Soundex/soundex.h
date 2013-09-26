#ifndef SOUNDEX_H_INCLUDED
#define SOUNDEX_H_INCLUDED

#include <string>
#include <map>
#include <vector>
#include <utility>

class SoundexCoder {
public:
	SoundexCoder(size_t length);

	std::string code(const std::string& str) const;
	bool doSoundSimilarly(const std::string& s1, const std::string& s2) const;

private:
	static const int INVALID_CHAR = -1;
	static std::string lowerAlphabet;
	static std::string upperAlphabet;

	void initLettersMap();
	void initReplacements();


	int letterValue(char c) const;
	static void toLower(std::string* s);
	void makeReplacements(std::string* s) const;

	std::map<char, int> lettersMap_;
	std::vector< std::pair<std::string, std::string> > replacements_;
	size_t length_;
};


#endif //SOUNDEX_H_INCLUDED
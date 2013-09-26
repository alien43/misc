#include "soundex.h"

#include <utility>
#include <vector>

using std::string;
using std::pair;
using std::make_pair;
using std::vector;

string SoundexCoder::lowerAlphabet = "àáâãäå¸æçèéêëìíîïğñòóôõö÷øùúûüışÿ";
string SoundexCoder::upperAlphabet = "ÀÁÂÃÄÅ¨ÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß";

SoundexCoder::SoundexCoder(size_t length)
: length_(length)
{
	initLettersMap();
	initReplacements();
}


void SoundexCoder::initLettersMap()
{
	vector< pair<string, int> > values;
	values.push_back(make_pair("àîóûıÿ¸şèåé", 0));
	values.push_back(make_pair("áïâô", 1));
	values.push_back(make_pair("ãêäòõ", 2));
	values.push_back(make_pair("ëğ", 3));
	values.push_back(make_pair("ìí", 4));
	values.push_back(make_pair("çñö", 5));
	values.push_back(make_pair("æø÷ù", 6));
	values.push_back(make_pair("úü", 7));

	lettersMap_.clear();
	for (size_t iPair = 0; iPair < values.size(); ++iPair) {
		const string& currentString = values[iPair].first;
		const int currentValue = values[iPair].second;
		for (size_t iChar = 0; iChar < currentString.length(); ++iChar) {
			lettersMap_.insert(make_pair(currentString[iChar], currentValue));
		}
	}
}

void SoundexCoder::initReplacements()
{
	replacements_.clear();
	replacements_.push_back(make_pair("ú", ""));
	replacements_.push_back(make_pair("ü", ""));
	replacements_.push_back(make_pair("éà", "ÿ"));
	replacements_.push_back(make_pair("éî", "¸"));
	replacements_.push_back(make_pair("éó", "ş"));
	replacements_.push_back(make_pair("éå", "å"));
	replacements_.push_back(make_pair("éÿ", "ÿ"));
	replacements_.push_back(make_pair("òñ", "ö"));
	replacements_.push_back(make_pair("òüñ", "ö"));

}

int SoundexCoder::letterValue(char c) const
{
	const std::map<char, int>::const_iterator it = lettersMap_.find(c);
	if (it == lettersMap_.end()) {
		return INVALID_CHAR;
	} else {
		return it->second;
	}
}

void SoundexCoder::toLower(std::string* s)
{
	const size_t nLetters = upperAlphabet.length();
	for (size_t iChar = 0; iChar < s->length(); ++iChar) {
		int pos = -1;
		for (size_t iLetter = 0; iLetter < nLetters; ++iLetter)
		{
			if (s->at(iChar) == upperAlphabet[iLetter]) {
				pos = iLetter;
				break;
			}
		}
		if (pos != -1) {
			s->at(iChar) = lowerAlphabet[pos];
		}
	}
}

void SoundexCoder::makeReplacements(string* s) const
{
	for (size_t i = 0; i < replacements_.size(); ++i) {
		const string& strFrom = replacements_[i].first;
		const string& strTo = replacements_[i].second;
		bool isFound = true;
		while (isFound) {
			const size_t pos = s->find(strFrom);
			if (pos != string::npos) {
				s->replace(pos, strFrom.length(), strTo);
			} else {
				isFound = false;
			}
		}
	}
}

char digitToChar(int digit)
{
	return '0' + digit;
}

string SoundexCoder::code(const string& str) const
{
	string s = str;
	toLower(&s);
	makeReplacements(&s);
	string result = "";
	if (s.length() > 0) {
		result += s[0];
	}
	size_t i = 1;
	int previousCode = 0;
	while (i < s.length()) {
		const int letterCode = letterValue(s[i]);
		if (letterCode == INVALID_CHAR) {
			return "Bad input string";
		} else if (letterCode != 0) {
			if (previousCode != letterCode) {
				result += digitToChar(letterCode);
				previousCode = letterCode;
			}
		}
		++i;
	}
	result += string(length_, digitToChar(0));
	result = result.substr(0, length_ + 1);
	return result;
}

bool SoundexCoder::doSoundSimilarly(const string& s1, const string& s2) const
{
	return (code(s1) == code(s2));
}

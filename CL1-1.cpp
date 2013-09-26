#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <utility>

using namespace std;

const char* INPUT_FILENAME = "input.txt";
const char* OUTPUT_FILENAME = "output.txt";

const string RUSSIAN_ALPHABET = "àáâãäå¸æçèéêëìíîïğñòóôõö÷øùúûüışÿ"
    "ÀÁÂÃÄÅ¨ÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß";

typedef vector<string> Dictionary;
typedef vector<string> WordList;
typedef pair<string, int> SearchRecord;
typedef vector<SearchRecord> SearchDictionary;

void ReadDictionary(istream& inputStream, Dictionary* dictionary)
{
    int dictSize;
    inputStream >> dictSize >> ws;
    dictionary->resize(dictSize);
    for (int i = 0; i < dictSize; ++i) {
        getline(inputStream, dictionary->at(i));
    }    
}

void FindNewWords(const string& word, WordList* newWords, int nLettersToRemove, 
    const string& prefix = "")
{
    if (nLettersToRemove == 0 || word.length() == 0) {
        newWords->push_back(prefix + word);
    } else {
        // We can either delete first letter
        FindNewWords(word.substr(1), newWords, nLettersToRemove - 1, prefix);
        // or not
        FindNewWords(word.substr(1), newWords, nLettersToRemove, prefix + word[0]);
    }
    // Leave only one copy of each string
    sort(newWords->begin(), newWords->end());
    newWords->resize(
        unique(newWords->begin(), newWords->end()) - newWords->begin());
}

void CreateSearchDictionary(const Dictionary& dict,
    SearchDictionary* searchDict, int nLettersToRemove)
{
    searchDict->clear();
    for (size_t iWord = 0; iWord < dict.size(); ++iWord) {
        WordList newWords;
        FindNewWords(dict[iWord], &newWords, nLettersToRemove);
        for (size_t iNewWord = 0; iNewWord < newWords.size(); ++iNewWord) {
            searchDict->push_back(make_pair(newWords[iNewWord], iWord));
        }
    }
    sort(searchDict->begin(), searchDict->end());
}

bool ReadWord(istream& inputStream, string* word)
{
    word->assign("");
    bool isWord = true;
    char c;
    while ((c = inputStream.peek()) && isWord && !inputStream.eof()) {
        isWord = (RUSSIAN_ALPHABET.find(c) != string::npos);
        if (isWord) {
            word->push_back(c);
            inputStream.get();
        }
    }
    return (word->size() > 0);
}

bool ReadExtraChars(istream& inputStream, string* extraChars)
{
    extraChars->assign("");
    bool isExtraChar = true;
    char c;
    while ((c = inputStream.peek()) && isExtraChar && !inputStream.eof()) {
        isExtraChar = (RUSSIAN_ALPHABET.find(c) == string::npos);
        if (isExtraChar) {
            extraChars->push_back(c);
            inputStream.get();
        }
    }
    return (extraChars->size() > 0);
}

int LevenshteinDistance(const string& s1, const string& s2) {
    const int size1 = (int)s1.size();
    const int size2 = (int)s2.size();
    vector< vector<int> > matrix(size1 + 1);
    for (int i = 0; i <= size1; ++i) {
        matrix[i].resize(size2 + 1);
    }
    
    for (int i = 0; i <= size1; ++i) {
        matrix[i][0] = i;
    }
    for (int j = 0; j <= size2; ++j) {
        matrix[0][j] = j;
    }

    for (int i = 1; i <= size1; ++i)
    for (int j = 1; j <= size2; ++j) {
        if (s1[i - 1] == s2[j - 1]) {
            matrix[i][j] = matrix[i - 1][j - 1];
        } else {
            matrix[i][j] = min(matrix[i - 1][j - 1],
                min(matrix[i - 1][j], matrix[i][j - 1])) + 1;
        }
    }
    return matrix[size1][size2];
}

string FindCorrectedWord(const string& word, const Dictionary& dict, 
    const SearchDictionary& searchDict, int maxDistance)
{
    WordList newWords;
    FindNewWords(word, &newWords, maxDistance);
    const struct StringComparator {
        bool operator() (const SearchRecord& lhs, const SearchRecord& rhs) const {
            return lhs.first < rhs.first;
        }
    } stringComparator;

    vector<int> candidates;
    for (size_t iWord = 0; iWord < newWords.size(); ++iWord) {
        pair<SearchDictionary::const_iterator,
            SearchDictionary::const_iterator> bounds =
            equal_range(searchDict.begin(), searchDict.end(),
            make_pair(newWords[iWord], 0), stringComparator);
        while (bounds.first < bounds.second) {
            candidates.push_back(bounds.first->second);
            ++bounds.first;
        }
    }
    // Leave unique candidates
    sort(candidates.begin(), candidates.end());
    candidates.resize(
        unique(candidates.begin(), candidates.end()) - candidates.begin());
    vector<int> dictIndexes;
    for (size_t i = 0; i < candidates.size(); ++i) {
        const string dictWord = dict[candidates[i]];
        if (LevenshteinDistance(word, dictWord) <= maxDistance) {
            dictIndexes.push_back(candidates[i]);
        }
    }

    string answer;
    if (dictIndexes.size() == 0) {
        answer = word;
    } else if (dictIndexes.size() == 1) {
        answer = dict[dictIndexes[0]];
    } else {
        answer = "{";
        for (size_t i = 0; i < dictIndexes.size(); ++i) {
            answer += dict[dictIndexes[i]];
            if (i < dictIndexes.size() - 1) {
                answer += ",";
            }
        }
        answer += "}";
    }
    return answer;
}

int main()
{
    const int maxDistance = 2;
    ifstream inputFile(INPUT_FILENAME);
    Dictionary dictionary;
    ReadDictionary(inputFile, &dictionary);
    SearchDictionary searchDictionary;
    CreateSearchDictionary(dictionary, &searchDictionary, maxDistance);
    //ostream outputFile(cout.rdbuf());
    ofstream outputFile(OUTPUT_FILENAME);
    while (!inputFile.eof()) {
        string extraChars;
        if (ReadExtraChars(inputFile, &extraChars)) {
            outputFile << extraChars;
        }
        string word;
        if (ReadWord(inputFile, &word)) {
            outputFile << FindCorrectedWord(
                word, dictionary, searchDictionary, maxDistance);
        }
    }
    outputFile.close();
    inputFile.close();
    return 0;
}

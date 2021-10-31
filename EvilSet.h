#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>

using namespace std;

class EvilSet {
public:
	// Creates an EvilSet with the given words
	explicit EvilSet(vector<string> words);

	// Filters master by picking words which match the most common pattern for the given character.
	// returns the chosen pattern
	string filter(char c);

	// returns the number of words in the set
	int numWords();

	// returns a random word from the set
	string getRandomWord();

private:
	// the words in the set
	vector<string> master;

	// returns the pattern for str given match
	// pattern is str with non-match chars replaced with underscores
	string getPattern(string str, char match);
};
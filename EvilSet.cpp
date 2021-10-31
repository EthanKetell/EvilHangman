#include "EvilSet.h"

EvilSet::EvilSet(vector<string> words) : master(words) {}

string EvilSet::filter(char c) {

	// map patterns to lists of words matching those patterns
	unordered_map<string, vector<string>> patterns;
	for (string word : master) {
		patterns[getPattern(word, c)].push_back(word);
	}

	// now search for the pattern with the most matches
	// a linear search is fine, since it won't be more than a dozen-ish patterns
	string maxPattern;
	int maxSize = 0;
	for (auto pair : patterns) {
		if (pair.second.size() > maxSize) {
			maxPattern = pair.first;
			maxSize = pair.second.size();
		}
	}
	master = patterns[maxPattern];	// set master to the list of words matching the best pattern
	return maxPattern;				// return the pattern
}

int EvilSet::numWords() {
	return master.size();
}

string EvilSet::getRandomWord() {
	srand(time(NULL));
	return master.at(rand()%master.size());
}

string EvilSet::getPattern(string str, char match) {
	for (char &c : str) {
		if (c != match) {
			c = '_';
		}
	}
	return str;
}

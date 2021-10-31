
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <climits>
#include <sstream>
#include <regex>

#include "dict.h"
#include "EvilSet.h"

using namespace std;

bool debug;

// Prompts the user to answer a yes or no question
bool PromptYesNoMessage(const string& msg) {
    while (true) {
        cout << msg << "(y/n) ";
        string response;
        cin >> response;
        if (response == "y")
            return true;
        if (response == "n")
            return false;
        cout << "Please enter y or n..." << endl;
    }
}

// Prompts the user for a positive integer
int PromptInt(const string& msg) {
    int num;
    while(true) {
        cout << msg << flush;
        cin >> num;					// attempt to read a number
        bool bad = cin.fail();		// save fail flag
        cin.clear();				// clear flags
        cin.ignore(INT_MAX, '\n');	// empty buffer
        if (bad) {	// The user entered something that wasn't a number
            cout << "Please enter a number." << endl;
        } else if (num <= 0) {
            cout << "Please enter a positive number." << endl;
        } else {
            return num;
        }
    }
}

// Prompts the user to enter a single char a-z not in guessedLetters
char PromptGuess(string &guessedLetters) {
    while (true) {
        cout << "Enter your guess: ";
        string guess;
        getline(cin, guess);
        for (char &c : guess) c = tolower(c); // tolower(guess)
        if (guess.length() != 1) {									// guess is not a single char
            cout << "Please enter a single character." << endl;
        } else if (!isalpha(guess[0])) {							// character is not a-z
            cout << "Please enter a letter (a-z)." << endl;
        } else if (guessedLetters.find(guess) != string::npos) {	// character exists in guessedLetters
            cout << "You already guessed that!" << endl;
        } else {
            return guess[0];
        }
    }
}

// handles selecting the word list from the given file and user length choice
vector<string> SelectWords(int &length) {
    // map of lists of words, where index for a word is its length
    // using a map ensures all possible word lengths fit
    unordered_map<int, vector<string>> wordsByLength;
    stringstream file;
    file.rdbuf()->pubsetbuf((char *) DICT, sizeof(DICT));
    string line;
    cout << "Loading dictionary...";
    smatch match;
    regex trim = regex(R"(\w+)");
    while (getline(file, line)) {						// for every line in the file
        if(regex_search(line, match, trim)) {
            line = match[0];
            wordsByLength[line.length()].push_back(line);    // add it to the map at key length
        }
    }
    cout << " Done!" << endl;
    while (true) {
        length = PromptInt("Enter word length: ");
        if (wordsByLength[length].empty()) {	// if there are no words with specified length
            cout << "Sorry, no words of that length exist." << endl;	// tell them
        } else {
            return wordsByLength[length];	// otherwise, return the list of words with specified length
        }
    }
}

// applies the given pattern to str
// copies non-underscore chars from pattern to string
// applyPattern("__s_", "t__t") ==> "t_st"
void applyPattern(string &str, string &pattern) {
    for (int i = 0; i < str.size(); i++) {	// str and pattern are the same length
        if (pattern[i] != '_') {			// if pattern has a non-underscore at i
            str[i] = pattern[i];			// copy it to str
        }
    }
}

void PlayEvilHangman() {
    // the length of the word
    int length;

    // the set of words that are still valid
    EvilSet words(SelectWords(length));

    // the blanked out word-so-far
    string progress;
    for (int i = 0; i < length; i++) {
        progress += "_";
    }

    // the number of guesses the player has remaining
    int numGuesses = PromptInt("How many guesses would you like? ");

    // the letters the player has guessed
    string guessedLetters;

    // game loop
    while (true) {
        cout << endl;
        for (char c : progress) {	// print the progress
            cout << c << " ";		// using a loop for spaces
        }

        if (!guessedLetters.empty()) {	// if any guesses have been made, show them
            cout << endl << "Guesses: ";
            for (char c : guessedLetters) {
                cout << c << " ";
            }
        }
        cout << endl << "Guesses left: " << numGuesses << endl;
        if (debug) {
            cout << "Words left: " << words.numWords() << endl;
        }

        char guess = PromptGuess(guessedLetters);	// get user's guess
        guessedLetters += guess;					// add it to guessedLetters
        string pattern = words.filter(guess);		// filter the list and store the resulting pattern
        applyPattern(progress, pattern);			// add the pattern to progress
        if (pattern.find(guess) == string::npos) {	// if the pattern has no matches
            numGuesses--;							// decrease the number of remaining guesses
        }
        if (progress.find('_') == string::npos) {	// if there are no empty spots left in the word
            cout << "Wow! You won!" << endl;		// the user won
            break;
        }
        if (numGuesses == 0) {						// if they ran out of guesses
            cout << "Sorry, you lost." << endl;		// they lost
            cout << "The word was: " << words.getRandomWord() << endl;	// so we lie to them
            break;
        }
    }
}

int main() {
    while (true) {
        PlayEvilHangman();
        if (!PromptYesNoMessage("Would you like to play again?"))
            break;
    }
}
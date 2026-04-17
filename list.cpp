#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::cin;
using std::cout;

string getWord() {
    string word;
    cin >> word;
    return word;
}
    
void loadWords(string body, vector<string>& wordbook) {
    std::ifstream file(body);
    string line;
    while (std::getline(file, line)) {
        for (char& staff : line) {
            staff = std::tolower(static_cast<unsigned char>(staff));
        }
        wordbook.push_back(line);
    }
}

void complete(string& stub, vector<string>& wordbook, vector<string>& completions, int& tell) {
    for (string& word : wordbook) {
        if (word.starts_with(stub)) {
            completions.push_back(word);
        }
        tell++;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "No corpus argument provided" << std::endl;
        return 1;
    }

    vector<string> wordbook;
    loadWords(argv[1], wordbook);

    cout << wordbook.size() << " words loaded" << std::endl;
    
    while (true) {
        cout << "Enter (part of) a word: " << std::flush;
        string word = getWord();

        vector<string> completions;
        int tell = 0;
        complete(word, wordbook, completions, tell);
        
        if (completions.empty()) {
            cout << "No suggestions found" << std::endl;
        } else {
            cout << completions.size() << " matches in " << tell << " operations" << std::endl;
            int n = std::min(9, (int)completions.size());
            cout << "Top " << n << " suggestions:\n";
            for (int i = 0; i < n; i++) {
                cout << completions[i] << '\n';
            }
            cout << std::flush;
        }
    }
}

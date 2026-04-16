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

struct TrieNode {
    char part;
    vector<TrieNode> children;

    TrieNode(char c) : part(c) {
        // Good enough
        children.reserve(30);
    }

    void nodes(int& n) {
        for (TrieNode& child : children) {
            child.nodes(n);
        }
        n++;
    }

    void size(int& n) {
        for (TrieNode& child : children) {
            child.size(n);
        }
        n += sizeof(TrieNode);
    }

    void build(string stub, vector<string>& words) {
        for (TrieNode& child : children) {
            if (child.part == '\0') {
                words.push_back(stub);
            }
            child.build(stub + child.part, words);
        }
        return;
    }

    bool operator==(const TrieNode& other) const {
        return part == other.part;
    }
};

class Trie {
    TrieNode root;
    
public:
    int words;

    Trie() : root(TrieNode('\0')), words(0) {}

    int nodes() {
        int n;
        for (TrieNode& child : root.children) {
            child.nodes(n);
        }
        return n;
    }

    int size() {
        int n;
        for (TrieNode& child : root.children) {
            child.size(n);
        }
        return n;
    }

    void addWord(const string& word) {
        TrieNode* here = &root;
        // Go over the word
        for (char staff : word) {
            // Navigate the tree and check for the letter
            for (TrieNode& child : here->children) {
                if (child.part == staff) {
                    here = &child;
                    // I literally don't know how better to do this
                    goto next;
                }
            }
            // If the letter isn't found, add it
            here->children.push_back(TrieNode(staff));
            here = &here->children.back();
            next: // Continue the outer loop
        }
        if (std::find(here->children.begin(), here->children.end(), TrieNode('\0')) == here->children.end()) {
            // Null terminator marks full keys
            here->children.push_back(TrieNode('\0'));
            // If this block doesn't run, the word was a duplicate and so the counter need not be incremented
            words++;
        }
    }

    TrieNode* seek(string word){
        TrieNode* here = &root;
        // Go over the word
        for (char staff : word) {
            // Navigate the tree
            for (TrieNode& child : here->children) {
                if (child.part == staff) {
                    here = &child;
                    // Still don't know how better to do this
                    goto next;
                }
            }
            return nullptr;
            next: // If we had labelled breaks like Rust this wouldn't be a problem
        }
        return here;
    }

    vector<string> complete(string stub) {
        vector<string> words;
        TrieNode* ord = seek(stub);
        if (ord == nullptr) {
            return words;
        }
        ord->build(stub, words);
        // Sort by length
        std::ranges::sort(words, {}, &std::string::size);
        return words;
    }

    void loadWords(string body) {
        std::ifstream file(body);
        string line;
        while (std::getline(file, line)) {
            for (char& staff : line) {
                // Bleh
                staff = std::tolower(static_cast<unsigned char>(staff));
                addWord(line);
            }
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "No corpus argument provided" << std::endl;
        return 1;
    }

    Trie myTrie;
    myTrie.loadWords(argv[1]);
    
    cout << "Trie holds " << myTrie.words << " words over " << myTrie.nodes() << " nodes" << std::endl;
    
    while (true) {
        cout << "Enter (part of) a word: " << std::flush;
        string word = getWord();

        vector<string> completions = myTrie.complete(word);
        if (completions.empty()) {
            cout << "No suggestions found" << std::endl;
        } else {
            cout << completions.size() << " matches" << std::endl;
            int n = std::min(9, (int)completions.size());
            cout << "Top " << n << " suggestions:\n";
            for (int i = 0; i < n; i++) {
                cout << completions[i] << '\n';
            }
            cout << std::flush;
        }
    }
}


#include <iostream>
#include <fstream>
#include <utility>
#include "mklinkedlist.h"
#include "mkarray.h"

namespace lab3 {
    struct LLWord {
        std::shared_ptr<MKLL::Node<char>> begin;
        std::shared_ptr<MKLL::Node<char>> end;

        size_t length() {
            auto i = begin;
            size_t len = 1;
            while (i != end) {
                i = i->next;
                len++;
            }
            return len;
        }

        bool operator==(const LLWord &rhs) const {
            auto i = begin;
            auto j = rhs.begin;
            do {
                if (i->getval() != j->getval()) return false;
                if (i == end or j == rhs.end) return i == end and j == rhs.end;
                i = i->next;
                j = j->next;
            } while (true);
        }

        bool operator!=(const LLWord &rhs) const {
            return !(*this == rhs);
        }

        friend std::ostream &operator<<(std::ostream &os, const LLWord &word) {
            auto i = word.begin;
            while (i != word.end) {
                os << i->getval();
                i = i->next;
            }
            os << i->getval();
            return os;
        }
    };

    bool isalphabetical(const LLWord &w) {
        char c = 'a';
        auto i = w.begin;
        while (i != w.end) {
            if (i->getval() != c) return false;
            i = i->next;
            c++;
        };
        return i->getval() == c;
    }

    MKLL::DoubleLinkedList<lab3::LLWord> read_ll(MKLL::DoubleLinkedList<char> &buffer) {
        std::ifstream in("in.txt");
        in >> std::noskipws;
        MKLL::DoubleLinkedList<lab3::LLWord> wordlist;
        char c;
        while (true) {
            lab3::LLWord w;
            // Skip space
            while (in >> c and isspace(c)) {}
            // Still no symbol => eof
            if (!isgraph(c) or in.eof()) return wordlist;

            // Now c contains non-space - append as first symbol of word
            buffer.push_back(c);
            w.begin = buffer.end();
            // Read word till space
            while (in >> c and !isspace(c)) {
                buffer.push_back(c);
            }
            // End word
            w.end = buffer.end();
            // Register word
            wordlist.push_back(w);
            // Add delimiter to buffer
            buffer.push_back(' ');
        }
    }

    void linkedlist_task() {
        MKLL::DoubleLinkedList<char> buffer;
        MKLL::DoubleLinkedList<lab3::LLWord> wordlist = read_ll(buffer);

        if (wordlist.isempty()) {
            std::cout << "Еггор: Empty input" << std::endl;
            return;
        }
        lab3::LLWord firstword = wordlist.begin()->getval();
        std::cout << firstword << std::endl;
        for (auto w = wordlist.begin()->next; bool(w); w = w->next) {
            lab3::LLWord &word = w->getval();
            if (word != firstword and lab3::isalphabetical(word)) {
                // Simultaneously insert to buffer and move end of the word
                word.end = buffer.insert_after(word.end, '.');
                // Remove first letter
                buffer.remove(word.begin); // remove from buffer
                word.begin = word.begin->next; // move beginning of the word
                // Add period at the end: s
                std::cout << "edited: ";
            }
            std::cout << word << std::endl;
        }
    }

    struct ArrWord {
        MKAR::Array<char> * arr;
        size_t begin;
        size_t end;

        ArrWord() : arr(nullptr) {}
        ArrWord(MKAR::Array<char> &arr) : arr(&arr) {}

        size_t length() {
            return end - begin + 1;
        }

        bool operator==(const ArrWord &rhs) const {
            auto i = begin;
            auto j = rhs.begin;
            do {
                if ((*arr)[i] != (*rhs.arr)[i]) return false;
                if (i == end or j == rhs.end) return i == end and j == rhs.end;
                i++;
                j++;
            } while (true);
        }

        bool operator!=(const ArrWord &rhs) const {
            return !(*this == rhs);
        }

        friend std::ostream &operator<<(std::ostream &os, const ArrWord &word) {
            auto i = word.begin;
            while (i != word.end) {
                os << (*word.arr)[i];
                i++;
            }
            os << (*word.arr)[i];
            return os;
        }
    };

    bool isalphabetical(const ArrWord &w) {
        char c = 'a';
        auto i = w.begin;
        while (i != w.end) {
            if ((*w.arr)[i] != c) return false;
            i++;
            c++;
        };
        return (*w.arr)[i] == c;
    }

    MKAR::Array<ArrWord> read_arr(MKAR::Array<char> &buffer) {
        std::ifstream in("in.txt");
        in >> std::noskipws;
        MKAR::Array<lab3::ArrWord> wordlist(20);
        char c;
        while (true) {
            lab3::ArrWord w(buffer);
            // Skip space
            while (in >> c and isspace(c)) {}
            // Still no symbol => eof
            if (!isgraph(c) or in.eof()) return wordlist;

            // Now c contains non-space - append as first symbol of word
            w.begin = buffer.push_back(c);
            // Read word till space
            while (in >> c and !isspace(c)) {
                buffer.push_back(c);
            }
            // End word
            w.end = buffer.len-1;
            // Register word
            wordlist.push_back(w);
            // Add delimiter to buffer
            buffer.push_back(' ');
        }
    }

    void array_task() {
        MKAR::Array<char> buffer(256);
        MKAR::Array<ArrWord> wordlist = read_arr(buffer);

        if (wordlist.isempty()) {
            std::cout << "Еггор: Empty input" << std::endl;
            return;
        }
        ArrWord firstword = wordlist[0];
        std::cout << firstword << std::endl;
        for (size_t i = 1; i < wordlist.len; i++) {
            lab3::ArrWord &word = wordlist[i];
            if (word != firstword and lab3::isalphabetical(word)) {
                // Simultaneously insert to buffer and move end of the word
                word.end = buffer.insert_after(word.end, '.');
                // Remove first letter
                buffer.remove(word.begin); // remove from buffer
                // Add period at the end: s
                std::cout << "edited: ";
            }
            std::cout << word << std::endl;
        }
    }
}

int main() {

    lab3::linkedlist_task();
    lab3::array_task();

    return 0;
}
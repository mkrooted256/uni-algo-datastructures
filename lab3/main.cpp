#include <iostream>
#include <fstream>
#include <utility>
#include "mklinkedlist.h"

namespace lab3 {
    struct Word {
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

        bool operator==(const Word &rhs) const {
            auto i = begin;
            auto j = rhs.begin;
            do {
                if (i->getval() != j->getval()) return false;
                if (i == end or j == rhs.end) return i == end and j == rhs.end;
                i = i->next;
                j = j->next;
            } while (true);
        }

        bool operator!=(const Word &rhs) const {
            return !(*this == rhs);
        }

        friend std::ostream &operator<<(std::ostream &os, const Word &word) {
            auto i = word.begin;
            while (i != word.end) {
                os << i->getval();
                i = i->next;
            }
            os << i->getval();
            return os;
        }
    };

    bool isalphabetical(const Word &w) {
        char c = 'a';
        auto i = w.begin;
        while (i != w.end) {
            if (i->getval() != c) return false;
            i = i->next;
            c++;
        };
        return i->getval() == c;
    }


    MKLL::DoubleLinkedList<lab3::Word> read(MKLL::DoubleLinkedList<char> &buffer) {
        std::ifstream in("in.txt");
        in >> std::noskipws;
        MKLL::DoubleLinkedList<lab3::Word> wordlist;
        char c;
        while (true) {
            lab3::Word w;
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
        MKLL::DoubleLinkedList<lab3::Word> wordlist = read(buffer);

        if (wordlist.isempty()) {
            std::cout << "Еггор: Empty input" << std::endl;
            return;
        }
        lab3::Word firstword = wordlist.begin()->getval();
        std::cout << firstword << std::endl;
        for (auto w = wordlist.begin()->next; bool(w); w = w->next) {
            lab3::Word& word = w->getval();
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

}

int main() {
    lab3::linkedlist_task();

    return 0;
}
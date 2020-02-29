#ifndef LAB3_MKLINKEDLIST_H
#define LAB3_MKLINKEDLIST_H

#include <memory>
#include <algorithm>

namespace MKLL {

    template<class T>
    class Node {
        T val;
    public:
        std::shared_ptr<Node<T>> prev;
        std::shared_ptr<Node<T>> next;

        explicit Node(T val) : val(val) {
            prev = next = nullptr;
        }

        T& getval() {
            return val;
        }

        bool islast() { return !bool(next); }
        bool isfirst() { return !bool(prev); }
    };

    template <class T>
    class DoubleLinkedList {
    public:
        std::shared_ptr<Node<T>> _begin;
        std::shared_ptr<Node<T>> _end;
        DoubleLinkedList(): _begin(nullptr), _end(nullptr) {}

        std::shared_ptr<Node<T>> push_back(T v) {
            if (isempty()) {
                return _begin = _end = std::shared_ptr<Node<T>>(new Node<T>(v));
            } else {
                _end->next = std::shared_ptr<Node<T>>(new Node<T>(v));
                _end->next->prev = _end;
                _end = _end->next;
                return _end;
            }
        }

        std::shared_ptr<Node<T>> push_front(T v) {
            if (isempty()) {
                return _begin = _end = std::shared_ptr<Node<T>>(new Node<T>(v));
            } else {
                _begin->prev = std::shared_ptr<Node<T>>(new Node<T>(v));
                _begin->prev->next = _begin;
                _begin = _begin->prev;
                return _begin;
            }
        }

        std::shared_ptr<Node<T>> insert_before(std::shared_ptr<Node<T>> i, T v) {
            std::shared_ptr<Node<T>> n = std::shared_ptr<Node<T>>(new Node<T>(v));
            n->next = i;
            n->prev = i->prev;
            if (!i->isfirst())
                i->prev->next = n;
            i->prev = n;
            return n;
        }
        std::shared_ptr<Node<T>> insert_after(std::shared_ptr<Node<T>> i, T v) {
            std::shared_ptr<Node<T>> n = std::shared_ptr<Node<T>>(new Node<T>(v));
            n->next = i->next;
            n->prev = i;
            if (!i->islast())
                i->next->prev = n;
            i->next = n;
            return n;
        }

        bool remove(std::shared_ptr<Node<T>> i) {
            if (!i->islast())
                i->next->prev = i->prev;
            if (!i->isfirst())
                i->prev->next = i->next;
            if (_end == i)
                _end = i->prev;
            if (_begin == i)
                _begin = i->next;
        }

        bool isempty() {
            return !_begin;
        }

        std::shared_ptr<Node<T>> begin() const {
            return _begin;
        }

        std::shared_ptr<Node<T>> end() const {
            return _end;
        }
    };
};

#endif //LAB3_MKLINKEDLIST_H

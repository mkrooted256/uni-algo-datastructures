#ifndef LAB3_MKSTACK_H
#define LAB3_MKSTACK_H

#include "mklinkedlist.h"
#include "mkarray.h"

namespace MK {
    // Abstract queue or stack
    template <class T, class iter>
    class QS {
    public:
        virtual iter push(T) = 0;
        virtual T pop() = 0;
        virtual bool isempty() = 0;
    };
}

// LinkedList variant
namespace MKLL {
    template <class T>
    class Queue : public MK::QS<T, std::shared_ptr<MKLL::Node<T>>> {
    public:
        MKLL::DoubleLinkedList<T> list;

        Queue(): list() {}
        explicit Queue(const MKLL::DoubleLinkedList<T> &list) : list(list) {}

        std::shared_ptr<MKLL::Node<T>> push(T v) {
            return list.push_front(v);
        }

        T pop() {
            T v = std::move(list.end()->getval());
            list.remove(list.end());
            return v;
        }

        bool isempty() {
            return list.isempty();
        }
    };

    template <class T>
    class Stack : public MK::QS<T, std::shared_ptr<MKLL::Node<T>>> {
    public:
        MKLL::DoubleLinkedList<T> list;
        explicit Stack(const MKLL::DoubleLinkedList<T> &list) : list(list) {}
        Stack() : list() {}

        std::shared_ptr<MKLL::Node<T>> push(T v) {
            return list.push_back(v);
        }

        T pop() {
            T v = std::move(list.end()->getval());
            list.remove(list.end());
            return v;
        }

        bool isempty() {
            return list.isempty();
        }
    };
}



// Array variant
namespace MKAR {
    template <class T>
    class Queue : public MK::QS<T, size_t> {
    public:
        MKAR::Array<T> arr;
        explicit Queue(const MKAR::Array<T> &arr) : arr(arr) {}

        size_t push(T v) {
            return arr.insert_before(0,v);
        }

        T pop() {
            T v = std::move(arr[arr.len-1]);
            arr.remove(arr.len-1);
            return v;
        }

        bool isempty() override {
            return arr.isempty();
        }
    };

    template <class T>
    class Stack : public MK::QS<T, size_t> {
    public:
        MKAR::Array<T> arr;
        explicit Stack(const MKAR::Array<T> &arr) : arr(arr) {}

        size_t push(T v) {
            return arr.push_back(v);
        }

        T pop() {
            T v = std::move(arr[arr.len-1]);
            arr.remove(arr.len-1);
            return v;
        }

        bool isempty() override {
            return arr.isempty();
        }
    };
}

#endif //LAB3_MKSTACK_H

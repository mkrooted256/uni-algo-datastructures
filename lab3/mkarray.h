#ifndef LAB3_MKARRAY_H
#define LAB3_MKARRAY_H

#include <memory>
#include <algorithm>

namespace MKAR {
    template<class T>
    class Array {
    public:
        T* mem;
        size_t len, size;

        Array(size_t size) : len(0), size(size) {
            mem = new T[size];
        }

        ~Array() {
            delete [] mem;
        }

        void shift_right(size_t start) {
            if (len < size and len>0) {
                len++;
                for (size_t j = len; j > start; j--) {
                    mem[j] = mem[j - 1];
                }
            }
        }
        void shift_left(size_t start) {
            if (len < size and len>0) {
                len--;
                for (size_t j = start; j < len; j++) {
                    mem[j] = mem[j+1];
                }
            }
        }

        size_t push_back(const T &v) {
            if (len < size) {
                mem[len] = v;
                len++;
                return len - 1;
            }
            return -1;
        }

        size_t insert_after(size_t i, const T &v) {
            if (len < size) {
                shift_right(i+1);
                mem[i+1] = v;
                return i + 1;
            }
            return -1;
        }

        size_t insert_before(size_t i, const T &v) {
            if (len < size) {
                shift_right(i);
                mem[i] = v;
                return i;
            }
            return -1;
        }

        bool remove(size_t i) {
            shift_left(i);
        }

        bool isempty() {
            return !len;
        }

        T& operator[](size_t i) {
            return mem[i];
        }
    };
};

#endif //LAB3_MKARRAY_H

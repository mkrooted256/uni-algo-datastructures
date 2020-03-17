#include <utility>
#include <functional>
#include "mklinkedlist.h"
#include "mkstack.h"

#ifndef LAB3_MATHPARSER_H
#define LAB3_MATHPARSER_H

typedef int t;
struct statement_node {
    enum types {
        VAL, OP
    } type;

    t val;
    std::string op;

    statement_node(t val) : type(VAL), val(val), op() {}
    statement_node(std::string op) : type(OP), val(0), op(std::move(op)) {}
};

std::function<t(t, t)> operation_resolver(const std::string &op) {
    if (op == "*") {
        return [](t l, t r) { return l * r; };
    }
    if (op == "/") {
        return [](t l, t r) { return l / r; };
    }
    if (op == "+") {
        return [](t l, t r) { return l + r; };
    }
    if (op == "-") {
        return [](t l, t r) { return l - r; };
    }
    return [](t l, t r) { return 0; };
}

bool isop(char c) {
    const char ops[] = "+-/*";
    for (auto o: ops)
        if (c==o) return true;
    return false;
}

struct IllegalState : public std::exception {
    char c;
    int i;
    std::string details;

    IllegalState(char c, int i, std::string details): c(c), i(i), details(std::move(details)) {}
    IllegalState(char c, int i): c(c), i(i), details() {}
};

class MathStatement {
public:
    MKLL::Stack<statement_node> line;

    void build_from_infix(const std::string &str) {
        line = MKLL::Stack<statement_node>();

        char c;
        int depth = 0;
        int i = 0;
        int len = str.length();

        MKLL::Stack<statement_node> ops;   // Operations
        int opslen = 0;

        while ( i < len ) {
            c = str[i];
//            if (i != 0 and opslen == 0 and depth == 0) break;
            if (c == '(') {
                depth++;
            } else if (c == ')') {
                if (depth == 0) throw IllegalState(c,i,"Unexpected null depth");
                if (opslen == 0) throw IllegalState(c,i,"Operations stack empty");
                depth--;
                line.push(ops.pop());
                opslen--;
            } else if (std::isdigit(c)) {
                t val = c - '0';
                while (std::isdigit(str[i+1])) {
                    c=str[++i];
                    val = val*10 + c - '0';
                }
                line.push(statement_node(val));
            } else if (isop(c)) {
                char opstring[]{c,0};
                ops.push(statement_node(opstring));
                opslen++;
            } else
                throw IllegalState(c,i,"Invalid character");
            i++;
        }
        if (opslen == 1)
            line.push(ops.pop());
        else
            throw IllegalState(str[i],i,"Suspicious - operation stack not empty");

    }

    t compute() {
        if (line.isempty()) throw std::exception();

        t result = 0;
        auto i = line.list.begin();
        auto computer = MKLL::Stack<statement_node>();
        int complength = 0;
        while (i.get()) {
            auto node = i->getval();
            if (node.type == statement_node::VAL) {
                computer.push(node);
                complength++;
            } else {
                // Operation
                auto r = computer.pop().val;
                auto l = computer.pop().val;
                computer.push(statement_node(operation_resolver(node.op)(l, r)));
                complength--;
            }
            i = i->next;
        }
        if (complength == 1) return computer.pop().val;
        throw IllegalState(0,0,"Final computer length != 1");
    }
};

class MathParser {

};


#endif //LAB3_MATHPARSER_H

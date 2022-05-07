#ifndef NODE_H
#define NODE_H

#include <string>
#include "token.h"
using namespace std;

enum Label { PROGRAM, VARS, BLOCK, EXPR, N, A, M, R, STATS, MSTAT, STAT, IN, OUT,
             IF, LOOP1, LOOP2, ASSIGN, R0, LABEL, GOTO, ERROR };

struct Node {
    Label code;
    string label;
    Node* child1;
    Node* child2;
    Node* child3;
    Node* child4;
    token tok1;
    token tok2;
    token tok3;
    token tok4;
    token tok5;
};

Node* getNode(string, Label);

#endif
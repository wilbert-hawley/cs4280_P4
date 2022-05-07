#ifndef SEMANTIC_H
#define SEMANTIC_H
#include "parser.h"
#include "node.h"
#include "token.h"
using namespace std;

void errorScript(token, string);
void pop();
void push(token tok, int);
void staticSem(Node*, int, int);
#endif
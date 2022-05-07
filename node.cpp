#include "node.h"
using namespace std;

Node* getNode(string name, Label lab) {
    Node* newNode = new Node;
    newNode->label = name;
    newNode->code = lab;
    newNode->child1 = newNode->child2 = newNode->child3 = newNode->child4 = NULL;
    return newNode;
}
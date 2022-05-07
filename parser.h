#include "scanner.h"
#include "node.h"
using namespace std;

void parse_error();
void printTree(Node*, int);
Node* parser(fstream&);
Node* program_(fstream&);
Node* vars_(fstream&);
Node* block_(fstream&);
Node* expr_(fstream&);
Node* N_(fstream&);
Node* N1_(fstream&);
Node* A_(fstream&);
Node* M_(fstream&);
Node* R_(fstream&);
Node* stats_(fstream&);
Node* mStat_(fstream&);
Node* stat_(fstream&);
Node* in_(fstream&);
Node* out_(fstream&);
Node* if_(fstream&);
Node* loop1_(fstream&);
Node* loop2_(fstream&);
Node* assign_(fstream&);
Node* R0_(fstream&);
Node* label_(fstream&);
Node* goto_(fstream&);


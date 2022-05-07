#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <string>
#include <fstream>
#include "token.h"
using namespace std;


token tkConstruct(int, string, int);
int lookupCol(char, int&);
char skip(fstream&, int&);
int filter(fstream&, char&, int&);
token scanner(fstream&, int&, char&);

#endif

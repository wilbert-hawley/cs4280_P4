#ifndef FILEOP_H
#define FILEOP_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
using namespace std;

void errorMessage(string, string, string);
void buildFile(fstream &, string str);

void emptyFileTest(fstream&, string);

#endif
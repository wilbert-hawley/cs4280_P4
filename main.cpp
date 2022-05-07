#include <iostream>
#include <string>
#include <fstream>
#include "fileOp.h"
#include "testscanner.h"
#include "scanner.h"
#include "parser.h"
#include "semantic.h"
using namespace std;

int main(int argc, char** argv) {

    fstream file;
    string filename;

    // Parsing input
    // If no arguments, reading from stdin
    if(argc == 1) {
        filename = "output";
        buildFile(file, filename + ".sp2022");
    }
        // If one argument, attempt to use filename provided
    else if(argc == 2) {
        filename = argv[1];
    }
        // Anymore arguments than 2 means there is an error.
    else {
        cout << "\"SCANNER ERROR: : Too many arguments. Abort.\n";
        return 1;
    }

    file.open((filename + ".sp2022").c_str(), fstream::in);

    // Test to make sure file opened successfully. If not, print message and quit
    if(!file)
        errorMessage("SCANNER ERROR: ", filename + ".sp2022 ", "failed to open");

    // Testing to see if the file is actually empty before using it.
    emptyFileTest(file, filename + ".sp2022");

    //testScanner(file);

    Node* root = parser(file);
    //printTree(root, 0);
    staticSem(root, 0, 0);

    cout << "File " << filename + ".sp2022 " << "appears to conform to the rules.\n";
    file.close();
    return 0;
}

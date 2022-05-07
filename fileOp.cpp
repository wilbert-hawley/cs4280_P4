#include "fileOp.h"
using namespace std;

void errorMessage(string location, string filename, string message) {
    cout << location << filename << message << endl;
    exit(1);
}

void buildFile(fstream &fi, string str) {
    fi.open(str.c_str(), fstream::out);
    if(!fi) {
        errorMessage("SCANNER ERROR: ", str, " failed to open");
    }
    cout << "Reading in from std and creating temporary file. If typing, use ctrl-D on it's "
         << "own line to simulate end of file.\n";
    string temp;

    while(getline(cin, temp)) {
        fi << temp.append("\n");
    }

    fi.close();
}

void emptyFileTest(fstream& file, string filename) {

    string input;
    int i = 0;

    while(getline(file, input)) {
        i++;
    }

    if(i == 0){
        errorMessage("SCANNER ERROR: ", filename, " was empty. Abort.");
    }
    //file << "\n\n";
    file.close();
    file.open(filename.c_str(), fstream::in);
}

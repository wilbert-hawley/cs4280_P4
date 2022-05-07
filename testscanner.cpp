#include "testscanner.h"
#include "scanner.h"

/*string tokenNames[] ={ "Identifier", "Keyword", "Number", "EOF", "Error", "Start", "Stop",
                       "Repeat", "Until", "Whole", "Label", "Quit", "Listen", "Yell", "Main", "Portal",
                       "If", "Then", "Pick", "Declare", "Assign", "Proc", "Equal Sign", "Double Equal",
                       "Less Than", "Greater Than", "Not Equal To", "Colon", "Colon Equals", "Plus Sign",
                       "Minus Sign", "Multiplication Sign", "Slash", "Modulus", "Period", "Left Parenthesis",
                       "Right Parenthesis", "Comma", "Left Curly Brace", "Right Curly Brace", "Semicolon",
                       "Left Bracket", "Right Bracket" };*/



void testScanner(fstream& file) {
        bool flag = true;
        token currentTK;
        int lineNumber = 0;
        char nextChar;
        file.get(nextChar);
        /*while(!file.eof()) {
                currentTK = scanner(file, lineNumber, nextChar);
                if(currentTK.tk == EOF_tk)
                    flag = false;
                cout << "TokenID: " << tokenNames[currentTK.tk] << "     TokenInstance: " << currentTK.instance << "     LineNumber: " << currentTK.line << endl;
        }

        if(flag){
            currentTK = tkConstruct(1003, "EOF", lineNumber);
                cout << "TokenID: " << tokenNames[currentTK.tk] << "     TokenInstance: " << currentTK.instance << "     LineNumber: " << currentTK.line << endl;
        }*/

        cout << "Scan Done\n";
}
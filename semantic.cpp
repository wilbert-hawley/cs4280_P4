#include <iostream>
#include "semantic.h"
using namespace std;

string tokenNames2[] ={ "Identifier", "Keyword", "Number", "EOF", "Error", "Start", "Stop",
                       "Repeat", "Until", "Whole", "Label", "Quit", "Listen", "Yell", "Main", "Portal",
                       "If", "Then", "Pick", "Declare", "Assign", "Proc", "Equal Sign", "Double Equal",
                       "Less Than", "Greater Than", "Not Equal To", "Colon", "Colon Equals", "Plus Sign",
                       "Minus Sign", "Multiplication Sign", "Slash", "Modulus", "Period", "Left Parenthesis",
                       "Right Parenthesis", "Comma", "Left Curly Brace", "Right Curly Brace", "Semicolon",
                       "Left Bracket", "Right Bracket" };

token stack[100];
int totalCount = 0;
int scope = 0;


void errorScript(token tok, string label) {
    cout << label << ":: Found error when processing " << tokenNames2[tok.tk] << "     TokenInstance: "
         << tok.instance << "     LineNumber: " << tok.line << endl;
    exit(1);
}

int verify(token tok) {
    //cout << "VERIFY TokenID: " << tokenNames2[tok.tk] << "     TokenInstance: " << tok.instance << "     LineNumber: " << tok.line << endl;

    if(totalCount < 1) {
        cout << "VERIFY Tried to verify when stack was empty\n";
        exit(1);
    }

    for(int i = totalCount - 1; i >= 0; i--) {
        if(stack[i].instance == tok.instance) {

            //cout << "Found " << tok.instance << " and distance = " << totalCount - i - 1 << endl;
            return totalCount - i - 1;
        }
    }
    //cout << "DIDN'T FIND~~~~~~~~~~\n";
    return -1;
}

void pop(int sCount) {
    //cout << "POP varCount = " << varCount << endl;
    //cout << "Total = " << totalCount << endl;
    for(int i = 0; i < 100; i++) {
        if(stack[i].scope == sCount) {
            //cout << "Popping " << stack[i].instance << endl;
            stack[i].instance = "";
            totalCount--;
            if(totalCount < 1) {
                break;
            }
        }
    }
}
void push(token tok, int scopeCount) {
    if(totalCount < 100) {
        //cout << "PUSH   TokenID: " << tokenNames2[tok.tk] << "     TokenInstance: " << tok.instance << "     LineNumber: " << tok.line << endl;
        totalCount++;
        //cout << "           scopeCOUNT = " << scopeCount << " TOTALCOUNT = " << totalCount << endl;
        tok.scope = scopeCount;
        for(int i = 0; i < 100; i++) {
            if(stack[i].instance == tok.instance) {
                if (stack[i].scope == tok.scope) {
                    cout << "SEMANTIC:: Error. Variable " << tok.instance
                         << " already declared in this scope. Abort.\n";
                    exit(1);
                }
            }
            if(stack[i].instance == "") {
                stack[i] = tok;
                break;
            }
        }
    }
    else {
        cout << "SEMANTIC:: Overflow - too many variables. Abort.\n";
        exit(1);
    }
}

/*enum Label { PROGRAM, VARS, BLOCK, EXPR, N, A, M, R, STATS, MSTAT, STAT, IN, OUT,
    IF, LOOP1, LOOP2, ASSIGN, R0, LABEL, GOTO, ERROR }; */

void staticSem(Node* node, int sCount, fstream& target) {

    if(node == NULL) {
        return;
    }
    switch(node->code) {
        case PROGRAM:   //<program> -> <vars> main <block>
            //cout << "Found PROGRAM\n";
            sCount = 0;
            staticSem(node->child1, sCount, target);
            staticSem(node->child2, sCount, target);
            /*for(int i = 0; i < totalCount + 1; i++) {
                cout << stack[i].instance << endl;
            }*/
            pop(0);
            return;

        case BLOCK:     // <block> -> { <vars> <stats> }
            //cout << "Found BLOCK\n";
            sCount++;
            staticSem(node->child1, sCount, target);
            staticSem(node->child2, sCount, target);
            pop(sCount);
            return;

        case VARS:      //<vars> -> empty | declare Identifier :=  whole ; <vars>
            //cout << "Found VARS\n";
            //varCount++;
            //cout << "Identifier " << node->tok2.instance << " found in VARS. VARCOUNT = " << varCount << endl;
            push(node->tok2, sCount);
            //cout << "varCount = " << varCount << endl;
            staticSem(node->child1, sCount, target);
            return;

        case EXPR:      // <expr> -> <N> - <expr> | <N>
            //cout << "Found EXPR\n";
            if(node->tok1.tk == SUB_tk) {
                staticSem(node->child1, sCount, target);
                staticSem(node->child2, sCount, target);
            }
            else
                staticSem(node->child1, sCount, target);
            return;
        case N:         // N -> <A> / <N> | <A> + <N> | <A>
            //cout << "Found N\n";
            if(node->tok1.tk == SLASH_tk || node->tok1.tk == SLASH_tk) {
                staticSem(node->child1, sCount, target);
                staticSem(node->child2, sCount, target);
            }
            else
                staticSem(node->child1, sCount, target);
            return;

        case A:         // <A> -> <M> * <A> | <M>
            //cout << "Found A\n";
            if(node->tok1.tk == MULT_tk) {
                staticSem(node->child1, sCount, target);
                staticSem(node->child2, sCount, target);
            }
            else
                staticSem(node->child1, sCount, target);
            return;

        case M:         // <M> -> % <M> |  <R>
            //cout << "Found M\n";
            staticSem(node->child1, sCount, target);
            return;

        case R:         // <R> -> ( <expr> ) | Identifier | Integer
            //cout << "Found R\n";
            if(node->tok1.tk == LPARA_tk) {
                staticSem(node->child1, sCount, target);
            }
            else if(node->tok1.tk == ID_tk) {
                //cout << "Found ID " << node->tok1.instance << " in R\n";
                if(node->tok1.instance != "") {
                    if (verify(node->tok1) < 0) {
                        errorScript(node->tok1, "R");
                    }
                }
            }
            return;

        case STATS:     // <stats> -> <stat>  <mStat>
            //cout << "Found STATS\n";
            staticSem(node->child1, sCount, target);
            staticSem(node->child2, sCount, target);
            return;

        case MSTAT:     // <mStat> -> empty |  <stat>  <mStat>
            //cout << "Found MSTAT\n";
            staticSem(node->child1, sCount, target);
            staticSem(node->child2, sCount, target);
            return;

        case STAT:      // <stat> -> <in> ; | <out> ; | <block> | <if> ; | <loop1> ; | <loop2> ; | <assign> ; | <goto> ; | <label> ;
            staticSem(node->child1, sCount, target);
            //cout << "Found STAT\n";
            return;

        case IN:        // <in> -> listen  Identifier
            //cout << "Found IN\n";
            if(verify(node->tok2) < 0) {
                errorScript(node->tok1, "R");
            }
            return;

        case OUT:       // <out> -> yell <expr>
            //cout << "Found OUT\n";
            staticSem(node->child1, sCount, target);
            return;

        case IF:        // <if> -> if [ <expr> <RO> <expr> ] then <stat>
            //cout << "Found IF\n";
            staticSem(node->child1, sCount, target);
            staticSem(node->child2, sCount, target);
            staticSem(node->child3, sCount, target);
            staticSem(node->child4, sCount, target);
            return;
        case LOOP1:     // <loop1> -> repeat  [ <expr> <RO> <expr> ]  <stat>
            //cout << "Found LOOP1\n";
            staticSem(node->child1, sCount, target);
            staticSem(node->child2, sCount, target);
            staticSem(node->child3, sCount, target);
            staticSem(node->child4, sCount, target);
            return;

        case LOOP2:     // <loop2> -> repeat <stat> until [ <expr> <RO> <expr> ]
            //cout << "Found LOOP2\n";
            staticSem(node->child1, sCount, target);
            staticSem(node->child2, sCount, target);
            staticSem(node->child3, sCount, target);
            staticSem(node->child4, sCount, target);
            return;

        case ASSIGN:    // <assign> -> assign Identifier  = <expr>
            //cout << "Found ASSIGN\n";
            if(verify(node->tok2) < 0) {
                errorScript(node->tok1, "R");
            }
            staticSem(node->child1, sCount, target);
            return;

        case R0:        // <RO> -> >=  | <= |  ==  |   ... (three tokens here) |  !=
            //cout << "Found R0\n";
            return;

        case LABEL:     // <label> -> label Identifier
            //cout << "Found LABEL\n";
            if(verify(node->tok2) < 0) {
                errorScript(node->tok1, "R");
            }
            return;
        case GOTO:      // <goto> ->  portal Identifier*/
            //cout << "Found GOTO\n";
            if(verify(node->tok2) < 0) {
                errorScript(node->tok1, "R");
            }
            return;

        case ERROR:
            cout << "HIT ERROR NODE\n";
            return;

        default:
            cout << "MAJOR ERROR\n";
            exit(1);
    }




}
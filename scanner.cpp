#include "token.h"

#include "scanner.h"
using namespace std;

enum colNum {
    LETTER, NUMBER, WS, EOF_COL, UNDER, EQUAL, LESS, GREAT, NOT, COL, PLUS, MINUS, MULTI,
    SLASH, MOD, DOT, LPARA, RPARA, COMMA, RCURLY, LCURLY, SEMI, RBRAK, LBRAK
};

enum internalState {
    s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16,
    s17, s18, s19, s20, s21, s22, s23, s24, s25, s26, s27
};

enum errorState {
    INVALID_CHAR = -1,
    INVALID_OP
};

enum finalStates {
    FINAL_ID = 1001, // ID_tk
    FINAL_NUM,
    FINAL_EOF,
    FINAL_EQUAL,
    FINAL_DUBEQU,
    FINAL_LESS,
    FINAL_GREAT,
    FINAL_NOTEQU,
    FINAL_COL,
    FINAL_COLEQU,
    FINAL_PLUS,
    FINAL_SUB,
    FINAL_MULT,
    FINAL_SLASH,
    FINAL_MOD,
    FINAL_DOT,
    FINAL_LPARA,
    FINAL_RPARA,
    FINAL_COMMA,
    FINAL_LCURLY,
    FINAL_RCURLY,
    FINAL_SEMI,
    FINAL_LBRAK,
    FINAL_RBRAK
};




int fsaTable[28][24] = {
        // 	       a-z           0-9	        ws          EOF             _              =             <               >              !             :              +              -              *              /              %              .              (              )              ,              {              }              ;              [              ]
        {	        s1,	          s2,           s0,    FINAL_EOF,           s1,           s4,           s6,            s8,           s10,           s12,           s14,           s15,           s16,           s17,           s18,           s19,           s20,           s21,           s22,           s24,           s23,           s25,           s27,           s26 },	// s0 (WS)
        {           s1,           s1,     FINAL_ID,     FINAL_ID,     FINAL_ID,     FINAL_ID,     FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID },  // s1 (letter)
        {    FINAL_NUM,           s2,    FINAL_NUM,    FINAL_NUM,    FINAL_NUM,    FINAL_NUM,    FINAL_NUM,     FINAL_NUM,     FINAL_NUM,     FINAL_NUM,     FINAL_NUM,     FINAL_NUM,     FINAL_NUM,     FINAL_NUM,     FINAL_NUM,     FINAL_NUM,     FINAL_NUM,     FINAL_NUM,     FINAL_NUM,     FINAL_NUM,     FINAL_NUM,     FINAL_NUM,     FINAL_NUM,     FINAL_NUM }, // s2 (numbers)
        {           s1,     FINAL_ID,     FINAL_ID,     FINAL_ID,     FINAL_ID,     FINAL_ID,     FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID,      FINAL_ID }, // s3 (_)
        {  FINAL_EQUAL,  FINAL_EQUAL,  FINAL_EQUAL,  FINAL_EQUAL,  FINAL_EQUAL,           s5,  FINAL_EQUAL,   FINAL_EQUAL,   FINAL_EQUAL,   FINAL_EQUAL,   FINAL_EQUAL,   FINAL_EQUAL,   FINAL_EQUAL,   FINAL_EQUAL,   FINAL_EQUAL,   FINAL_EQUAL,   FINAL_EQUAL,   FINAL_EQUAL,   FINAL_EQUAL,   FINAL_EQUAL,   FINAL_EQUAL,   FINAL_EQUAL,   FINAL_EQUAL,   FINAL_EQUAL }, // s4 (=)
        { FINAL_DUBEQU, FINAL_DUBEQU, FINAL_DUBEQU, FINAL_DUBEQU, FINAL_DUBEQU, FINAL_DUBEQU, FINAL_DUBEQU,  FINAL_DUBEQU,  FINAL_DUBEQU,  FINAL_DUBEQU,  FINAL_DUBEQU,  FINAL_DUBEQU,  FINAL_DUBEQU,  FINAL_DUBEQU,  FINAL_DUBEQU,  FINAL_DUBEQU,  FINAL_DUBEQU,  FINAL_DUBEQU,  FINAL_DUBEQU,  FINAL_DUBEQU,  FINAL_DUBEQU,  FINAL_DUBEQU,  FINAL_DUBEQU,  FINAL_DUBEQU }, // s5 (==)                                                       } // s4 (=)
        {   INVALID_OP,   INVALID_OP,   INVALID_OP,   INVALID_OP,   INVALID_OP,           s7,   INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,     INVALID_OP,   INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP }, // s6 (<)
        {   FINAL_LESS,   FINAL_LESS,   FINAL_LESS,   FINAL_LESS,   FINAL_LESS,   FINAL_LESS,   FINAL_LESS,    FINAL_LESS,    FINAL_LESS,    FINAL_LESS,    FINAL_LESS,    FINAL_LESS,    FINAL_LESS,    FINAL_LESS,    FINAL_LESS,    FINAL_LESS,    FINAL_LESS,    FINAL_LESS,    FINAL_LESS,    FINAL_LESS,    FINAL_LESS,    FINAL_LESS,    FINAL_LESS,    FINAL_LESS }, // s7 (<=)
        {   INVALID_OP,   INVALID_OP,   INVALID_OP,   INVALID_OP,   INVALID_OP,           s9,   INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP }, // s8
        {  FINAL_GREAT,  FINAL_GREAT,  FINAL_GREAT,  FINAL_GREAT,  FINAL_GREAT,  FINAL_GREAT,  FINAL_GREAT,   FINAL_GREAT,   FINAL_GREAT,   FINAL_GREAT,   FINAL_GREAT,   FINAL_GREAT,   FINAL_GREAT,   FINAL_GREAT,   FINAL_GREAT,   FINAL_GREAT,   FINAL_GREAT,   FINAL_GREAT,   FINAL_GREAT,   FINAL_GREAT,   FINAL_GREAT,   FINAL_GREAT,   FINAL_GREAT,   FINAL_GREAT }, // s9 (>=)
        {   INVALID_OP,   INVALID_OP,   INVALID_OP,   INVALID_OP,   INVALID_OP,          s11,   INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP,    INVALID_OP }, // s10 (!)
        { FINAL_NOTEQU, FINAL_NOTEQU, FINAL_NOTEQU, FINAL_NOTEQU, FINAL_NOTEQU, FINAL_NOTEQU, FINAL_NOTEQU,  FINAL_NOTEQU,  FINAL_NOTEQU,  FINAL_NOTEQU,  FINAL_NOTEQU,  FINAL_NOTEQU,  FINAL_NOTEQU,  FINAL_NOTEQU,  FINAL_NOTEQU,  FINAL_NOTEQU,  FINAL_NOTEQU,  FINAL_NOTEQU,  FINAL_NOTEQU,  FINAL_NOTEQU,  FINAL_NOTEQU,  FINAL_NOTEQU,  FINAL_NOTEQU,  FINAL_NOTEQU }, // s11 (!=)
        {    FINAL_COL,    FINAL_COL,    FINAL_COL,    FINAL_COL,    FINAL_COL,          s13,    FINAL_COL,     FINAL_COL,     FINAL_COL,     FINAL_COL,     FINAL_COL,     FINAL_COL,     FINAL_COL,     FINAL_COL,     FINAL_COL,     FINAL_COL,     FINAL_COL,     FINAL_COL,     FINAL_COL,     FINAL_COL,     FINAL_COL,     FINAL_COL,     FINAL_COL,     FINAL_COL }, // s12 (:)
        { FINAL_COLEQU, FINAL_COLEQU, FINAL_COLEQU, FINAL_COLEQU, FINAL_COLEQU, FINAL_COLEQU, FINAL_COLEQU,  FINAL_COLEQU,  FINAL_COLEQU,  FINAL_COLEQU,  FINAL_COLEQU,  FINAL_COLEQU,  FINAL_COLEQU,  FINAL_COLEQU,  FINAL_COLEQU,  FINAL_COLEQU,  FINAL_COLEQU,  FINAL_COLEQU,  FINAL_COLEQU,  FINAL_COLEQU,  FINAL_COLEQU,  FINAL_COLEQU,  FINAL_COLEQU,  FINAL_COLEQU }, // s13 (:=)
        {   FINAL_PLUS,   FINAL_PLUS,   FINAL_PLUS,   FINAL_PLUS,   FINAL_PLUS,   FINAL_PLUS,   FINAL_PLUS,    FINAL_PLUS,    FINAL_PLUS,    FINAL_PLUS,    FINAL_PLUS,    FINAL_PLUS,    FINAL_PLUS,    FINAL_PLUS,    FINAL_PLUS,    FINAL_PLUS,    FINAL_PLUS,    FINAL_PLUS,    FINAL_PLUS,    FINAL_PLUS,    FINAL_PLUS,    FINAL_PLUS,    FINAL_PLUS,    FINAL_PLUS }, // s14 (+)
        {    FINAL_SUB,    FINAL_SUB,    FINAL_SUB,    FINAL_SUB,    FINAL_SUB,    FINAL_SUB,    FINAL_SUB,     FINAL_SUB,     FINAL_SUB,     FINAL_SUB,     FINAL_SUB,     FINAL_SUB,     FINAL_SUB,     FINAL_SUB,     FINAL_SUB,     FINAL_SUB,     FINAL_SUB,     FINAL_SUB,     FINAL_SUB,     FINAL_SUB,     FINAL_SUB,     FINAL_SUB,     FINAL_SUB,     FINAL_SUB }, // s15 (-)
        {   FINAL_MULT,   FINAL_MULT,   FINAL_MULT,   FINAL_MULT,   FINAL_MULT,   FINAL_MULT,   FINAL_MULT,    FINAL_MULT,    FINAL_MULT,    FINAL_MULT,    FINAL_MULT,    FINAL_MULT,    FINAL_MULT,    FINAL_MULT,    FINAL_MULT,    FINAL_MULT,    FINAL_MULT,    FINAL_MULT,    FINAL_MULT,    FINAL_MULT,    FINAL_MULT,    FINAL_MULT,    FINAL_MULT,    FINAL_MULT }, // s16 (*)
        {  FINAL_SLASH,  FINAL_SLASH,  FINAL_SLASH,  FINAL_SLASH,  FINAL_SLASH,  FINAL_SLASH,  FINAL_SLASH,   FINAL_SLASH,   FINAL_SLASH,   FINAL_SLASH,   FINAL_SLASH,   FINAL_SLASH,   FINAL_SLASH,   FINAL_SLASH,   FINAL_SLASH,   FINAL_SLASH,   FINAL_SLASH,   FINAL_SLASH,   FINAL_SLASH,   FINAL_SLASH,   FINAL_SLASH,   FINAL_SLASH,   FINAL_SLASH,   FINAL_SLASH }, // s17 (/)
        {    FINAL_MOD,    FINAL_MOD,    FINAL_MOD,    FINAL_MOD,    FINAL_MOD,    FINAL_MOD,    FINAL_MOD,     FINAL_MOD,     FINAL_MOD,     FINAL_MOD,     FINAL_MOD,     FINAL_MOD,     FINAL_MOD,     FINAL_MOD,     FINAL_MOD,     FINAL_MOD,     FINAL_MOD,     FINAL_MOD,     FINAL_MOD,     FINAL_MOD,     FINAL_MOD,     FINAL_MOD,     FINAL_MOD,     FINAL_MOD }, //s18 (%)
        {    FINAL_DOT,    FINAL_DOT,    FINAL_DOT,    FINAL_DOT,    FINAL_DOT,    FINAL_DOT,    FINAL_DOT,     FINAL_DOT,     FINAL_DOT,     FINAL_DOT,     FINAL_DOT,     FINAL_DOT,     FINAL_DOT,     FINAL_DOT,     FINAL_DOT,     FINAL_DOT,     FINAL_DOT,     FINAL_DOT,     FINAL_DOT,     FINAL_DOT,     FINAL_DOT,     FINAL_DOT,     FINAL_DOT,     FINAL_DOT }, // s19 (.)
        {  FINAL_LPARA,  FINAL_LPARA,  FINAL_LPARA,  FINAL_LPARA,  FINAL_LPARA,  FINAL_LPARA,  FINAL_LPARA,   FINAL_LPARA,   FINAL_LPARA,   FINAL_LPARA,   FINAL_LPARA,   FINAL_LPARA,   FINAL_LPARA,   FINAL_LPARA,   FINAL_LPARA,   FINAL_LPARA,   FINAL_LPARA,   FINAL_LPARA,   FINAL_LPARA,   FINAL_LPARA,   FINAL_LPARA,   FINAL_LPARA,   FINAL_LPARA,   FINAL_LPARA }, // s20 (()
        {  FINAL_RPARA,  FINAL_RPARA,  FINAL_RPARA,  FINAL_RPARA,  FINAL_RPARA,  FINAL_RPARA,  FINAL_RPARA,   FINAL_RPARA,   FINAL_RPARA,   FINAL_RPARA,   FINAL_RPARA,   FINAL_RPARA,   FINAL_RPARA,   FINAL_RPARA,   FINAL_RPARA,   FINAL_RPARA,   FINAL_RPARA,   FINAL_RPARA,   FINAL_RPARA,   FINAL_RPARA,   FINAL_RPARA,   FINAL_RPARA,   FINAL_RPARA,   FINAL_RPARA }, // s21 ())
        {  FINAL_COMMA,  FINAL_COMMA,  FINAL_COMMA,  FINAL_COMMA,  FINAL_COMMA,  FINAL_COMMA,  FINAL_COMMA,   FINAL_COMMA,   FINAL_COMMA,   FINAL_COMMA,   FINAL_COMMA,   FINAL_COMMA,   FINAL_COMMA,   FINAL_COMMA,   FINAL_COMMA,   FINAL_COMMA,   FINAL_COMMA,   FINAL_COMMA,   FINAL_COMMA,   FINAL_COMMA,   FINAL_COMMA,   FINAL_COMMA,   FINAL_COMMA,   FINAL_COMMA }, // s22 (,)
        { FINAL_LCURLY, FINAL_LCURLY, FINAL_LCURLY, FINAL_LCURLY, FINAL_LCURLY, FINAL_LCURLY, FINAL_LCURLY,  FINAL_LCURLY,  FINAL_LCURLY,  FINAL_LCURLY,  FINAL_LCURLY,  FINAL_LCURLY,  FINAL_LCURLY,  FINAL_LCURLY,  FINAL_LCURLY,  FINAL_LCURLY,  FINAL_LCURLY,  FINAL_LCURLY,  FINAL_LCURLY,  FINAL_LCURLY,  FINAL_LCURLY,  FINAL_LCURLY,  FINAL_LCURLY,  FINAL_LCURLY }, // s23 ({)
        { FINAL_RCURLY, FINAL_RCURLY, FINAL_RCURLY, FINAL_RCURLY, FINAL_RCURLY, FINAL_RCURLY, FINAL_RCURLY,  FINAL_RCURLY,  FINAL_RCURLY,  FINAL_RCURLY,  FINAL_RCURLY,  FINAL_RCURLY,  FINAL_RCURLY,  FINAL_RCURLY,  FINAL_RCURLY,  FINAL_RCURLY,  FINAL_RCURLY,  FINAL_RCURLY,  FINAL_RCURLY,  FINAL_RCURLY,  FINAL_RCURLY,  FINAL_RCURLY,  FINAL_RCURLY,  FINAL_RCURLY }, // s24 (})
        {   FINAL_SEMI,   FINAL_SEMI,   FINAL_SEMI,   FINAL_SEMI,   FINAL_SEMI,   FINAL_SEMI,   FINAL_SEMI,    FINAL_SEMI,    FINAL_SEMI,    FINAL_SEMI,    FINAL_SEMI,    FINAL_SEMI,    FINAL_SEMI,    FINAL_SEMI,    FINAL_SEMI,    FINAL_SEMI,    FINAL_SEMI,    FINAL_SEMI,    FINAL_SEMI,    FINAL_SEMI,    FINAL_SEMI,    FINAL_SEMI,    FINAL_SEMI,    FINAL_SEMI }, // s25 (;)
        {  FINAL_LBRAK,  FINAL_LBRAK,  FINAL_LBRAK,  FINAL_LBRAK,  FINAL_LBRAK,  FINAL_LBRAK,  FINAL_LBRAK,   FINAL_LBRAK,   FINAL_LBRAK,   FINAL_LBRAK,   FINAL_LBRAK,   FINAL_LBRAK,   FINAL_LBRAK,   FINAL_LBRAK,   FINAL_LBRAK,   FINAL_LBRAK,   FINAL_LBRAK,   FINAL_LBRAK,   FINAL_LBRAK,   FINAL_LBRAK,   FINAL_LBRAK,   FINAL_LBRAK,   FINAL_LBRAK,   FINAL_LBRAK }, // s26 ([)
        {  FINAL_RBRAK,  FINAL_RBRAK,  FINAL_RBRAK,  FINAL_RBRAK,  FINAL_RBRAK,  FINAL_RBRAK,  FINAL_RBRAK,   FINAL_RBRAK,   FINAL_RBRAK,   FINAL_RBRAK,   FINAL_RBRAK,   FINAL_RBRAK,   FINAL_RBRAK,   FINAL_RBRAK,   FINAL_RBRAK,   FINAL_RBRAK,   FINAL_RBRAK,   FINAL_RBRAK,   FINAL_RBRAK,   FINAL_RBRAK,   FINAL_RBRAK,   FINAL_RBRAK,   FINAL_RBRAK,   FINAL_RBRAK } // s27 (])

};



int lookupCol(char col, int& lineNumber) {
    switch(col) {
        case '_':
            return UNDER;
        case '=':
            return EQUAL;
        case '<':
            return LESS;
        case '>':
            return GREAT;
        case '!':
            return NOT;
        case ':':
            return COL;
        case '+':
            return PLUS;
        case '-':
            return MINUS;
        case '*':
            return MULTI;
        case '/':
            return SLASH;
        case '%':
            return MOD;
        case '.':
            return DOT;
        case '(':
            return LPARA;
        case ')':
            return RPARA;
        case ',':
            return COMMA;
        case '}':
            return RCURLY;
        case '{':
            return LCURLY;
        case ';':
            return SEMI;
        case ']':
            return RBRAK;
        case '[':
            return LBRAK;
        default:
            if (isalpha(col) || col == '_')
                return LETTER;
            else if (isdigit(col))
                return NUMBER;
            else if (isspace(col)) {
                return WS;
            }
            else {
                cout << "SCANNER ERROR: hit invalid char " << col << "in lookupCol\n";
                exit(INVALID_CHAR);
            }
    }
}

// function to skip the file pointer beyond comments
// return the next char back to filter
char skip(fstream& file, int& lineNumber) {
    int i = 1;
    char next;

    while(i < 4) {
    // account for reaching end of file before comment ends
        file.get(next);
        //cout << "Next = " << next << endl;
        /*if(next == '\n')
            cout << "here\n";
            lineNumber++;*/
        if(next == '#')
            i++;
    }

    file.get(next);
    return next;
}

int filter(fstream& file, char& nextChar, int &lineNumber) {
    // check for EOF
    if(file.eof())
        return EOF_COL;

    switch(nextChar) {
        case '#':
            nextChar = skip(file, lineNumber);
            break;
        case '\n':
            lineNumber++;
            break;
    }

    return lookupCol(nextChar, lineNumber);
}




keyTokenPairs keyWords[] = { {"start", START_tk}, {"stop", STOP_tk}, {"repeat", REPEAT_tk},
                             {"until", UNTIL_tk}, {"whole", WHOLE_tk}, {"label", LABEL_tk},
                             { "quit", QUIT_tk}, {"listen", LISTEN_tk}, {"yell", YELL_tk},
                             {"main", MAIN_tk}, {"portal", PORTAL_tk}, {"if", IF_tk},
                             {"then", THEN_tk}, {"pick", PICK_tk}, {"declare", DECLARE_tk},
                             {"assign", ASSIGN_tk}, {"proc", PROC_tk} };


tokenID kwLookup (string candidate) {
    for(int i = 0; i < 17; i++) {
        if(keyWords[i].word == candidate)
            return keyWords[i].tkID;
    }

    return ID_tk;
}


token tkConstruct(int final, string inst, int line)
{
    token newToken;

    switch(final) {
        case FINAL_ID:
            newToken.tk = kwLookup(inst);
            break;
        case FINAL_NUM:
            newToken.tk = NUM_tk;
            break;
        case FINAL_EQUAL:
            newToken.tk = EQUAL_tk;
            break;
        case FINAL_DUBEQU:
            newToken.tk = DUBEQUAL_tk;
            break;
        case FINAL_LESS:
            newToken.tk = LESS_tk;
            break;
        case FINAL_GREAT:
            newToken.tk = GREAT_tk;
            break;
        case FINAL_NOTEQU:
            newToken.tk = NOTEQU_tk;
            break;
        case FINAL_COL:
            newToken.tk = COL_tk;
            break;
        case FINAL_COLEQU:
            newToken.tk = COLEQU_tk;
            break;
        case FINAL_PLUS:
            newToken.tk = PLUS_tk;
            break;
        case FINAL_SUB:
            newToken.tk = SUB_tk;
            break;
        case FINAL_MULT:
            newToken.tk = MULT_tk;
            break;
        case FINAL_SLASH:
            newToken.tk = SLASH_tk;
            break;
        case FINAL_MOD:
            newToken.tk = MOD_tk;
            break;
        case FINAL_DOT:
            newToken.tk = DOT_tk;
            break;
        case FINAL_LPARA:
            newToken.tk = LPARA_tk;
            break;
        case FINAL_RPARA:
            newToken.tk = RPARA_tk;
            break;
        case FINAL_COMMA:
            newToken.tk = COMMA_tk;
            break;
        case FINAL_LCURLY:
            newToken.tk = LCURLY_tk;
            break;
        case FINAL_RCURLY:
            newToken.tk = RCURLY_tk;
            break;
        case FINAL_SEMI:
            newToken.tk = SEMI_tk;
            break;
        case FINAL_LBRAK:
            newToken.tk = LBRAK_tk;
            break;
        case FINAL_RBRAK:
            newToken.tk = RBRAK_tk;
            break;
        case FINAL_EOF:
            newToken.tk = EOF_tk;
            newToken.instance = "EOF";
            newToken.line = line;
            return newToken;
            break;
        default:
            cout << "\"SCANNER ERROR: Unknown type enountered\n";
            exit(1);
    }

    newToken.instance = inst;
    newToken.line = line;

    return newToken;
}


token scanner(fstream& file, int& lineNumber, char& nextChar) {


    int state = 0,
        nextState = -1;
    string s;
    int col = 0;


    while(state < 1001) {

        // filter that skips comments and returns the column number of the next valid char
        col = filter(file, nextChar, lineNumber);

        // current state gives us the row
        // col is the column of the nextChar
        // nextState is returned from the table
        nextState = fsaTable[state][col];

        if (nextState < 0) {
            cout << "Error\n";
            s = "error";
            break;
        } else if (nextState > 1000)
            return tkConstruct(nextState, s, lineNumber);
        else {
            state = nextState;
            if(!isspace(nextChar))
                s.append(1, nextChar);
            file.get(nextChar);
        }
    }

    token errorToken = { ERR_tk, "Scanner error", lineNumber };
    return errorToken;
}

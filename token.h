#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
using namespace std;



enum tokenID {
    ID_tk, // Identifier token
    KEY_tk, // Keywork Token
    NUM_tk, // Number token
    EOF_tk,
    ERR_tk,
    START_tk,
    STOP_tk,
    REPEAT_tk,
    UNTIL_tk,
    WHOLE_tk,
    LABEL_tk,
    QUIT_tk,
    LISTEN_tk,
    YELL_tk,
    MAIN_tk,
    PORTAL_tk,
    IF_tk,
    THEN_tk,
    PICK_tk,
    DECLARE_tk,
    ASSIGN_tk,
    PROC_tk,
    EQUAL_tk,
    DUBEQUAL_tk,
    LESS_tk,
    GREAT_tk,
    NOTEQU_tk,
    COL_tk,
    COLEQU_tk,
    PLUS_tk,
    SUB_tk,
    MULT_tk,
    SLASH_tk,
    MOD_tk,
    DOT_tk,
    LPARA_tk,
    RPARA_tk,
    COMMA_tk,
    LCURLY_tk,
    RCURLY_tk,
    SEMI_tk,
    LBRAK_tk,
    RBRAK_tk
};

struct token {
    tokenID tk;
    string instance;
    int line;
    int scope;
};

struct keyTokenPairs {
    string word;
    tokenID tkID;
};

#endif


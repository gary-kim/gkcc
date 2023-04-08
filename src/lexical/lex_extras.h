#ifndef LEX_EXTRAS_H
#define LEX_EXTRAS_H

#include <stdbool.h>

#define MAX_STR_LENGTH 2048

extern char YY_FILENAME[MAX_STR_LENGTH];

union _yynums {
    long long      ylonglong;
    long double    ylongdouble;
    double         ydouble;
    float          yfloat;
    long           ylong;
    int            yint;
};

enum _yynum_type {
    YYNUM_TYPE_LONGLONG,
    YYNUM_TYPE_LONG_DOUBLE,
    YYNUM_TYPE_DOUBLE,
    YYNUM_TYPE_FLOAT,
    YYNUM_TYPE_LONG,
    YYNUM_TYPE_INT,
};

struct _yynum {
    enum _yynum_type type;
    bool is_unsigned;
    union _yynums num;
};


enum _yylval_type {
    YYLVAL_TYPE_NUMBER = 0,
    YYLVAL_TYPE_STRING,
    YYLVAL_TYPE_CHAR,
};

struct _yylval {
    union _data {
        struct _yynum number;
        struct _yystring {
            char string[MAX_STR_LENGTH];
            unsigned int length;
        } string;
        char character;
    } data;
    enum _yylval_type type;
};

// TODO: NOT THIS
#include "ast.h"

#endif

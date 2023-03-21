#include <stdbool.h>

const int MAX_STR_LENGTH = 2048;
#define MAX_STR_LENGTH 2048

extern unsigned long YY_LINE_NUMBER;

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
        char string[MAX_STR_LENGTH];
        char character;
    } data;
    enum _yylval_type type;
};

extern struct _yylval yylval;


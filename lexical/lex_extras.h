extern unsigned long YY_LINE_NUMBER;

union _yylval {
    unsigned long long yunsigned_longlong;
    long long      ylonglong;
    long double    ylong_double;
    double         y_double;
    float          yfloat;
    unsigned long  yunsigned_long;
    unsigned       yunsigned;
    long           ysigned_long;
    int            ysigned;
};

enum _yynum_type {
    YYNUM_TYPE_UNSIGNED_LONGLONG = 0,
    YYNUM_TYPE_LONGLONG,
    YYNUM_TYPE_LONG_DOUBLE,
    YYNUM_TYPE_DOUBLE,
    YYNUM_TYPE_FLOAT,
    YYNUM_TYPE_UNSIGNED_LONG,
    YYNUM_TYPE_UNSIGNED,
    YYNUM_TYPE_LONG,
    YYNUM_TYPE_INT
};

extern union _yylval yylval;
extern enum _yynum_type num_type;

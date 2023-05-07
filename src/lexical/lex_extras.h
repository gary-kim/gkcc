// Copyright (C) 2023 Gary Kim <gary@garykim.dev>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

#endif

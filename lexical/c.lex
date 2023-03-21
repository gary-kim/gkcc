%{
#include "ltests/tokens-manual.h"
#include "lex_extras.h"

/* Below is from https://github.com/westes/flex/blob/cf66c9e5f1af02c4b6f9fb5a10f83e28143a22d4/examples/manual/numbers.lex */
struct _yylval yylval;

%}

%option stack

%x SC_STRING
%x SC_CHAR

digit             [0-9]
hex_digit         [0-9a-fA-F]
oct_digit         [0-7]

exponent          [eE][+-]?{digit}+
i                 {digit}+
float_constant    ({i}\.{i}?|{i}?\.{i}){exponent}?
hex_constant      0[xX]{hex_digit}+
oct_constant      0{oct_digit}*
int_constant      {digit}+
long_ext          [lL]
longlong_ext      [lL][lL]
unsigned_ext      [uU]
float_ext         [fF]
ulong_ext         {long_ext}{unsigned_ext}|{unsigned_ext}{long_ext}
ulonglong_ext     {long_ext}{long_ext}{unsigned_ext}|{unsigned_ext}{long_ext}{long_ext}
alpha             [A-Za-z_]
alpha_num         ({alpha}|{digit})
identifier        {alpha}{alpha_num}*

%%
#.*\n  { /* Comment or preprocessor. do nothing */ }


<SC_STRING,SC_CHAR>[A-Za-z0-9] {
    yylval.data.string.string[yylval.data.string.length++] = yytext[0];
}

<SC_STRING>\" {
    yy_pop_state();
    return STRING;
}

<SC_CHAR>\' {
    yy_pop_state();
    if (yylval.data.string.length != 1) {
        fprintf(stderr, "Error: character has incorrect length");
        exit(1);
    }

    char character = yylval.data.string.string[0];
    yylval.type = YYLVAL_TYPE_CHAR;
    yylval.data.character = character;

    return CHAR;
}

\" {
yy_push_state(SC_STRING);
yylval.type = YYLVAL_TYPE_STRING;
yylval.data.string.length = 0;
}

\' {
// Characters start as a string then get converted later
yy_push_state(SC_CHAR);
yylval.type = YYLVAL_TYPE_STRING;
yylval.data.string.length = 0;
}

"auto" { return AUTO; }
"break" { return BREAK;  }
"case" { return CASE; }
"char" { return CHAR; }
"const" { return CONST; }
"continue" { return CONTINUE; }
"default" { return DEFAULT; }
"do" { return DO; }
"double" { return DOUBLE; }
"else" { return ELSE; }
"enum" { return ENUM; }
"extern" { return EXTERN; }
"float" { return FLOAT; }
"for" { return FOR; }
"goto" { return GOTO; }
"if" { return IF; }
"inline" { return INLINE; }
"int" { return INT; }
"long" { return LONG; }
"register" { return REGISTER; }
"restrict" { return RESTRICT; }
"return" { return RETURN; }
"short" { return SHORT; }
"signed" { return SIGNED; }
"sizeof" { return SIZEOF; }
"static" { return STATIC; }
"struct" { return STRUCT; }
"switch" { return SWITCH; }
"typedef" { return TYPEDEF; }
"union" { return UNION; }
"unsigned" { return UNSIGNED; }
"void" { return VOID; }
"volatile" { return VOLATILE; }
"while" { return WHILE; }
"_Bool" { return _BOOL; }
"_Complex" { return _COMPLEX; }
"_Imaginary" { return _IMAGINARY; }

"--"  { return MINUSMINUS;  }
"++"  { return PLUSPLUS;  }
"<="  { return LTEQ;  }
">="  { return GTEQ;  }
"=="  { return EQEQ;  }
"!="  { return NOTEQ;  }
"->"  { return INDSEL; }
"&&"  { return LOGAND; }
"<<=" { return SHLEQ; }
">>=" { return SHREQ; }
"<<"  { return SHL; }
">>"  { return SHR; }
"%="  { return MODEQ; }
"||"  { return LOGOR; }
"..." { return ELLIPSIS; }
"+="  { return PLUSEQ; }
"-="  { return MINUSEQ; }
"/="  { return DIVEQ; }
"*="  { return TIMESEQ; }
"^="  { return XOREQ; }
"|="  { return OREQ; }
"&="  { return ANDEQ; }

{identifier} {
    strncpy(yylval.data.string.string, yytext, MAX_STR_LENGTH);
    yylval.data.string.length = yyleng;
    yylval.type = YYLVAL_TYPE_STRING;
    return IDENT;
}

[ \t]*        /* do nothing *g/
\n            YY_LINE_NUMBER += 1;

  /* Below is from https://github.com/westes/flex/blob/cf66c9e5f1af02c4b6f9fb5a10f83e28143a22d4/examples/manual/numbers.lex */
{hex_constant}{ulong_ext} {  /* we need to skip the "0x" part */
                             sscanf(&yytext[2],"%lx",&yylval.data.number.num.ylong);
                             yylval.data.number.type = YYNUM_TYPE_LONG;
                             yylval.data.number.is_unsigned = true;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }
{hex_constant}{long_ext}  {
                             sscanf(&yytext[2],"%lx",&yylval.data.number.num.ylong);
                             yylval.data.number.type = YYNUM_TYPE_LONG;
                             yylval.data.number.is_unsigned = false;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }
{hex_constant}{unsigned_ext}  {
                             sscanf(&yytext[2],"%x",&yylval.data.number.num.yint);
                             yylval.data.number.type = YYNUM_TYPE_INT;
                             yylval.data.number.is_unsigned = true;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }
{hex_constant}            {
                             sscanf(&yytext[2],"%x",&yylval.data.number.num.yint);
                             yylval.data.number.type = YYNUM_TYPE_INT;
                             yylval.data.number.is_unsigned = false;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }
{oct_constant}{ulong_ext} {
                             sscanf(yytext,"%lo",&yylval.data.number.num.ylong);
                             yylval.data.number.type = YYNUM_TYPE_INT;
                             yylval.data.number.is_unsigned = true;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }
{oct_constant}{long_ext}  {
                             sscanf(yytext,"%lo",&yylval.data.number.num.ylong);
                             yylval.data.number.type = YYNUM_TYPE_LONG;
                             yylval.data.number.is_unsigned = false;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }
{oct_constant}{unsigned_ext}  {
                             sscanf(yytext,"%o",&yylval.data.number.num.yint);
                             yylval.data.number.type = YYNUM_TYPE_INT;
                             yylval.data.number.is_unsigned = true;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }
{oct_constant}            {
                             sscanf(yytext,"%o",&yylval.data.number.num.yint);
                             yylval.data.number.type = YYNUM_TYPE_INT;
                             yylval.data.number.is_unsigned = false;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }
{int_constant}{ulong_ext} {
                             sscanf(yytext,"%ld",&yylval.data.number.num.ylong);
                             yylval.data.number.type = YYNUM_TYPE_LONG;
                             yylval.data.number.is_unsigned = true;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }

{int_constant}{ulonglong_ext}  {
                             sscanf(yytext,"%llu",&yylval.data.number.num.ylonglong);
                             yylval.data.number.type = YYNUM_TYPE_LONGLONG;
                             yylval.data.number.is_unsigned = true;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }

{int_constant}{longlong_ext}  {
                             sscanf(yytext,"%ld",&yylval.data.number.num.ylong);
                             yylval.data.number.type = YYNUM_TYPE_LONG;
                             yylval.data.number.is_unsigned = false;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }

{int_constant}{long_ext}  {
                             sscanf(yytext,"%ld",&yylval.data.number.num.ylong);
                             yylval.data.number.type = YYNUM_TYPE_LONG;
                             yylval.data.number.is_unsigned = false;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }
{int_constant}{unsigned_ext}  {
                             sscanf(yytext,"%d",&yylval.data.number.num.yint);
                             yylval.data.number.type = YYNUM_TYPE_INT;
                             yylval.data.number.is_unsigned = true;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }
{int_constant}            {
                             sscanf(yytext,"%d",&yylval.data.number.num.yint);
                             yylval.data.number.type = YYNUM_TYPE_INT;
                             yylval.data.number.is_unsigned = false;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }
{float_constant}{long_ext}  {
                             sscanf(yytext,"%Lf",&yylval.data.number.num.ylongdouble);
                             yylval.data.number.type = YYNUM_TYPE_LONG_DOUBLE;
                             yylval.data.number.is_unsigned = false;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }
{float_constant}{float_ext}  {
                             sscanf(yytext,"%f",&yylval.data.number.num.yfloat);
                             yylval.data.number.type = YYNUM_TYPE_FLOAT;
                             yylval.data.number.is_unsigned = false;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }
{float_constant}          {
                             sscanf(yytext,"%lf",&yylval.data.number.num.ydouble);
                             yylval.data.number.type = YYNUM_TYPE_DOUBLE;
                             yylval.data.number.is_unsigned = false;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }


[*/+\-,^.;:()\[\]{}=&~!%<>|?]  { return(yytext[0]); }
. { fprintf(stderr, "ERROR!!\n"); }
%%

int yywrap(void){return 1;}

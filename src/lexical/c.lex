%{
#include "lex_extras.h"
#include "c.tab.h"

char YY_FILENAME[MAX_STR_LENGTH] = "<stdin>";
#define yylval yylval.yylval

%}

%option stack
%option yylineno
%option warn nodefault

%x SC_STRING
%x SC_CHAR
%x SC_PREPROCESSOR

digit              [0-9]
hex_digit          [0-9a-fA-F]
oct_digit          [0-7]

binary_exponent    [pP][+-]?{digit}+
exponent           [eE][+-]?{digit}+
i                  {digit}+
float_constant     (({i}\.{i}?|{i}?\.{i}){exponent}?)|({i}{exponent})
hex_constant       0[xX]{hex_digit}+
hex_float_constant 0[xX]({hex_digit}+|{hex_digit}*\.{hex_digit}+|{hex_digit}+\.){binary_exponent}
oct_constant       0{oct_digit}*
int_constant       [1-9]{digit}*
long_ext           [lL]
longlong_ext       [lL][lL]
unsigned_ext       [uU]
float_ext          [fF]
ulong_ext          {long_ext}{unsigned_ext}|{unsigned_ext}{long_ext}
ulonglong_ext      {long_ext}{long_ext}{unsigned_ext}|{unsigned_ext}{long_ext}{long_ext}
alpha              [A-Za-z_]
alpha_num          ({alpha}|{digit})
identifier         {alpha}{alpha_num}*

%%

#\ {i} {
    yy_push_state(SC_PREPROCESSOR);
    sscanf(&yytext[2], "%d", &yylineno);
}

<SC_PREPROCESSOR>\" {
    yy_push_state(SC_STRING);
    yylval.type = YYLVAL_TYPE_STRING;
    yylval.data.string.length = 0;
}

<SC_PREPROCESSOR>\n  {
    // Save current filename
    yylval.data.string.string[yylval.data.string.length] = '\0';
    strncpy(YY_FILENAME, yylval.data.string.string, MAX_STR_LENGTH);
    yylineno--;
    yy_pop_state();
}

<SC_PREPROCESSOR>. {
    /* empty */
}

<SC_STRING,SC_CHAR>[A-Za-z0-9\*\/\+\-\,\^\.\;\:\(\)\[\]\{\}\=\&\~\!\%\<\>\|\?] {
    yylval.data.string.string[yylval.data.string.length++] = yytext[0];
}

<SC_STRING,SC_CHAR>\\[0-7]+ {
    unsigned int decoded = 0;
    // Skip "\"
    sscanf(&yytext[1], "%o", &decoded);
    if ((decoded & 0xFFu) != decoded) {
      fprintf(stderr, "%s:%d:Warning:Octal sequence %s out of range\n", YY_FILENAME, yylineno, yytext);
      decoded = 0xFFu;
    }
    yylval.data.string.string[yylval.data.string.length++] = decoded;
}

<SC_STRING,SC_CHAR>\\x[0-9A-Fa-f]+ {
    unsigned int decoded = 0;
    // Skip "\x"
    sscanf(&yytext[2], "%x", &decoded);
    if ((decoded & 0xFFu) != decoded) {
      fprintf(stderr, "%s:%d:Warning:Hex sequence %s out of range\n", YY_FILENAME, yylineno, yytext);
      decoded = 0xFFu;
    }
    yylval.data.string.string[yylval.data.string.length++] = decoded;
}

<SC_CHAR>\" {
    yylval.data.string.string[yylval.data.string.length++] = '"';
}

<SC_STRING>\' {
    yylval.data.string.string[yylval.data.string.length++] = '\'';
}

<SC_STRING,SC_CHAR>\\\' {
    yylval.data.string.string[yylval.data.string.length++] = '\'';
}

<SC_STRING,SC_CHAR>\\\" {
    yylval.data.string.string[yylval.data.string.length++] = '"';
}

<SC_STRING,SC_CHAR>\\\? {
    yylval.data.string.string[yylval.data.string.length++] = '?';
}

<SC_STRING,SC_CHAR>\\\\ {
    yylval.data.string.string[yylval.data.string.length++] = '\\';
}

<SC_STRING,SC_CHAR>\\a {
    yylval.data.string.string[yylval.data.string.length++] = '\a';
}

<SC_STRING,SC_CHAR>\\b {
    yylval.data.string.string[yylval.data.string.length++] = '\b';
}

<SC_STRING,SC_CHAR>\\f {
    yylval.data.string.string[yylval.data.string.length++] = '\f';
}

<SC_STRING,SC_CHAR>\\n {
    yylval.data.string.string[yylval.data.string.length++] = '\n';
}

<SC_STRING,SC_CHAR>\\r {
     yylval.data.string.string[yylval.data.string.length++] = '\r';
}

<SC_STRING,SC_CHAR>\\t {
     yylval.data.string.string[yylval.data.string.length++] = '\t';
}

<SC_STRING,SC_CHAR>\\v {
     yylval.data.string.string[yylval.data.string.length++] = '\v';
}

<SC_STRING,SC_CHAR>\\0 {
    yylval.data.string.string[yylval.data.string.length++] = '\0';
}


<SC_STRING>\" {
    yy_pop_state();
    yylval.data.string.string[yylval.data.string.length] = '\0';
    if (YYSTATE == INITIAL)
        return STRING;
}

<SC_CHAR>\' {
    yy_pop_state();
    if (yylval.data.string.length != 1) {
        fprintf(stderr, "%s:%d:Warning:Unsupported multibyte character literal truncated to first byte\n", YY_FILENAME, yylineno);
    }

    char character = yylval.data.string.string[0];
    yylval.type = YYLVAL_TYPE_CHAR;
    yylval.data.character = character;

    if (YYSTATE == INITIAL)
        return CHARLIT;
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

[ \t\n]+ {
    /* do nothing */
}

    /* Below is from https://github.com/westes/flex/blob/cf66c9e5f1af02c4b6f9fb5a10f83e28143a22d4/examples/manual/numbers.lex */
    /* Handle all hexadecimal integer combinations */

{hex_constant}{ulonglong_ext} {
    sscanf(&yytext[2], "%llx", &yylval.data.number.num.ylonglong);
    yylval.data.number.type = YYNUM_TYPE_LONGLONG;
    yylval.data.number.is_unsigned = true;
    return NUMBER;
}

{hex_constant}{longlong_ext} {
    sscanf(&yytext[2], "%llx", &yylval.data.number.num.ylonglong);
    yylval.data.number.type = YYNUM_TYPE_LONGLONG;
    yylval.data.number.is_unsigned = false;
    return NUMBER;
}

{hex_constant}{ulong_ext} {  /* we need to skip the "0x" part */
    sscanf(&yytext[2],"%lx",&yylval.data.number.num.ylong);
    yylval.data.number.type = YYNUM_TYPE_LONG;
    yylval.data.number.is_unsigned = true;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}
{hex_constant}{long_ext}  {
    sscanf(&yytext[2],"%lx",&yylval.data.number.num.ylong);
    yylval.data.number.type = YYNUM_TYPE_LONG;
    yylval.data.number.is_unsigned = false;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}
{hex_constant}{unsigned_ext}  {
    sscanf(&yytext[2],"%x",&yylval.data.number.num.yint);
    yylval.data.number.type = YYNUM_TYPE_INT;
    yylval.data.number.is_unsigned = true;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}
{hex_constant}            {
    sscanf(&yytext[2],"%x",&yylval.data.number.num.yint);
    yylval.data.number.type = YYNUM_TYPE_INT;
    yylval.data.number.is_unsigned = false;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}

  /* Handle all octal integer constants */
{oct_constant}{ulonglong_ext} {
    sscanf(yytext, "%llo", &yylval.data.number.num.ylonglong);
    yylval.data.number.type = YYNUM_TYPE_LONGLONG;
    yylval.data.number.is_unsigned = true;
    return NUMBER;
}
{oct_constant}{longlong_ext} {
    sscanf(yytext, "%llo", &yylval.data.number.num.ylonglong);
    yylval.data.number.type = YYNUM_TYPE_LONGLONG;
    yylval.data.number.is_unsigned = false;
    return NUMBER;
}
{oct_constant}{ulong_ext} {
    sscanf(yytext,"%lo",&yylval.data.number.num.ylong);
    yylval.data.number.type = YYNUM_TYPE_LONG;
    yylval.data.number.is_unsigned = true;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}
{oct_constant}{long_ext}  {
    sscanf(yytext,"%lo",&yylval.data.number.num.ylong);
    yylval.data.number.type = YYNUM_TYPE_LONG;
    yylval.data.number.is_unsigned = false;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}
{oct_constant}{unsigned_ext}  {
    sscanf(yytext,"%o",&yylval.data.number.num.yint);
    yylval.data.number.type = YYNUM_TYPE_INT;
    yylval.data.number.is_unsigned = true;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}
{oct_constant}            {
    sscanf(yytext,"%o",&yylval.data.number.num.yint);
    yylval.data.number.type = YYNUM_TYPE_INT;
    yylval.data.number.is_unsigned = false;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}

    /* Handle all decimal integer constants */
{int_constant}{ulonglong_ext}  {
    sscanf(yytext,"%llu",&yylval.data.number.num.ylonglong);
    yylval.data.number.type = YYNUM_TYPE_LONGLONG;
    yylval.data.number.is_unsigned = true;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}

{int_constant}{longlong_ext}  {
    sscanf(yytext,"%ld",&yylval.data.number.num.ylong);
    yylval.data.number.type = YYNUM_TYPE_LONG;
    yylval.data.number.is_unsigned = false;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}
{int_constant}{ulong_ext} {
    sscanf(yytext,"%ld",&yylval.data.number.num.ylong);
    yylval.data.number.type = YYNUM_TYPE_LONG;
    yylval.data.number.is_unsigned = true;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}
{int_constant}{long_ext}  {
    sscanf(yytext,"%ld",&yylval.data.number.num.ylong);
    yylval.data.number.type = YYNUM_TYPE_LONG;
    yylval.data.number.is_unsigned = false;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}
{int_constant}{unsigned_ext}  {
    sscanf(yytext,"%d",&yylval.data.number.num.yint);
    yylval.data.number.type = YYNUM_TYPE_INT;
    yylval.data.number.is_unsigned = true;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}
{int_constant}            {
    sscanf(yytext,"%d",&yylval.data.number.num.yint);
    yylval.data.number.type = YYNUM_TYPE_INT;
    yylval.data.number.is_unsigned = false;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}

    /* Handle decimal float constants */
{float_constant}{long_ext}  {
    sscanf(yytext,"%Lf",&yylval.data.number.num.ylongdouble);
    yylval.data.number.type = YYNUM_TYPE_LONG_DOUBLE;
    yylval.data.number.is_unsigned = false;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}
{float_constant}{float_ext}  {
    sscanf(yytext,"%f",&yylval.data.number.num.yfloat);
    yylval.data.number.type = YYNUM_TYPE_FLOAT;
    yylval.data.number.is_unsigned = false;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}
{float_constant}          {
    sscanf(yytext,"%lf",&yylval.data.number.num.ydouble);
    yylval.data.number.type = YYNUM_TYPE_DOUBLE;
    yylval.data.number.is_unsigned = false;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}

    /* Handle hexadecimal float constants */
{hex_float_constant}{long_ext}  {
    sscanf(yytext,"%Lfx",&yylval.data.number.num.ylongdouble);
    yylval.data.number.type = YYNUM_TYPE_LONG_DOUBLE;
    yylval.data.number.is_unsigned = false;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}
{hex_float_constant}{float_ext}  {
    sscanf(yytext,"%fx",&yylval.data.number.num.yfloat);
    yylval.data.number.type = YYNUM_TYPE_FLOAT;
    yylval.data.number.is_unsigned = false;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}
{hex_float_constant}          {
    sscanf(yytext,"%lfx",&yylval.data.number.num.ydouble);
    yylval.data.number.type = YYNUM_TYPE_DOUBLE;
    yylval.data.number.is_unsigned = false;
    yylval.type = YYLVAL_TYPE_NUMBER;
    return NUMBER;
}

[\*\/\+\-\,\^\.\;\:\(\)\[\]\{\}\=\&\~\!\%\<\>\|\?]  { return(yytext[0]); }

. { fprintf(stderr, "ERROR!!\n"); }
%%

int yywrap(void){return 1;}

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

\" { 
    yy_push_state(SC_STRING);
    yylval.type = YYLVAL_TYPE_STRING;
}
\' {
    yy_push_state(SC_CHAR);
}
<SC_STRING,SC_CHAR>\" {
    int state = yy_top_state();
    yy_pop_state();
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

{identifier} { return IDENT; }

[ \t]*        /* do nothing */
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
                             sscanf(&yytext[2],"%lx",&yylval.ylong);
                             yylval.data.number.type = YYNUM_TYPE_LONG;
                             yylval.data.number.is_unsigned = false;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }
{hex_constant}{unsigned_ext}  {
                             sscanf(&yytext[2],"%x",&yylval.yint);
                             yylval.data.number.type = YYNUM_TYPE_INT;
                             yylval.data.number.is_unsigned = true;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }
{hex_constant}            {
                             sscanf(&yytext[2],"%x",&yylval.yint);
                             yylval.data.number.type = YYNUM_TYPE_INT;
                             yylval.data.number.is_unsigned = false;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }
{oct_constant}{ulong_ext} {
                             sscanf(yytext,"%lo",&yylval.ylong);
                             yylval.data.number.type = YYNUM_TYPE_INT;
                             yylval.data.number.is_unsigned = true;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }
{oct_constant}{long_ext}  {
                             sscanf(yytext,"%lo",&yylval.ysigned_long);
                             yylval.data.number.type = YYNUM_TYPE_LONG;
                             yylval.data.number.is_unsigned = false;
                             yylval.type = YYLVAL_TYPE_NUMBER;
                             return(NUMBER);
                          }
{oct_constant}{unsigned_ext}  {
                             sscanf(yytext,"%o",&yylval.yunsigned);\
                             num_type = YYNUM_TYPE_UNSIGNED;
                             return(NUMBER);
                          }
{oct_constant}            {
                             sscanf(yytext,"%o",&yylval.ysigned);
                             num_type = YYNUM_TYPE_INT;
                             return(NUMBER);
                          }
{int_constant}{ulong_ext} {
                             sscanf(yytext,"%ld",&yylval.yunsigned_long);
                             num_type = YYNUM_TYPE_UNSIGNED_LONG;
                             return(NUMBER);
                          }

{int_constant}{ulonglong_ext}  {
                             sscanf(yytext,"%llu",&yylval.yunsigned_longlong);
                             num_type = YYNUM_TYPE_UNSIGNED_LONGLONG;
                             return(NUMBER);
                          }

{int_constant}{longlong_ext}  {
                             sscanf(yytext,"%ld",&yylval.ysigned_long);
                             num_type = YYNUM_TYPE_LONG;
                             return(NUMBER);
                          }

{int_constant}{long_ext}  {
                             sscanf(yytext,"%ld",&yylval.ysigned_long);
                             num_type = YYNUM_TYPE_LONG;
                             return(NUMBER);
                          }
{int_constant}{unsigned_ext}  {
                             sscanf(yytext,"%d",&yylval.yunsigned);
                             num_type = YYNUM_TYPE_UNSIGNED;
                             return(NUMBER);
                          }
{int_constant}            {
                             sscanf(yytext,"%d",&yylval.ysigned);
                             num_type = YYNUM_TYPE_INT;
                             return(NUMBER);
                          }
{float_constant}{long_ext}  {
                             sscanf(yytext,"%Lf",&yylval.ylong_double);
                             num_type = YYNUM_TYPE_LONG_DOUBLE;
                             return(NUMBER);
                          }
{float_constant}{float_ext}  {
                             sscanf(yytext,"%f",&yylval.yfloat);
                             num_type = YYNUM_TYPE_FLOAT;
                             return(NUMBER);
                          }
{float_constant}          {
                             sscanf(yytext,"%lf",&yylval.y_double);
                             num_type = YYNUM_TYPE_DOUBLE;
                             return(NUMBER);
                          }


[*/+\-,^.;:()\[\]{}=&~!%<>|?]  { return(yytext[0]); }
. { fprintf(stderr, "ERROR!!\n"); }
%%

int yywrap(void){return 1;}

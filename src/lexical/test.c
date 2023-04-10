#include "lex.yy.h"
#include "lex_extras.h"
#include "c.tab.h"
#include <ctype.h>
#include <stdbool.h>

YYSTYPE yylval;

// Stopgap until proper replacement
#define yylval yylval.yylval

char buf[10];
char *TP_YYEOF = "YYEOF";
char *TP_IDENT = "IDENT";
char *TP_CHARLIT = "CHARLIT";
char *TP_STRING = "STRING";
char *TP_NUMBER = "NUMBER";
char *TP_INDSEL = "INDSEL";
char *TP_PLUSPLUS = "PLUSPLUS";
char *TP_MINUSMINUS = "MINUSMINUS";
char *TP_SHL = "SHL";
char *TP_SHR = "SHR";
char *TP_LTEQ = "LTEQ";
char *TP_GTEQ = "GTEQ";
char *TP_EQEQ = "EQEQ";
char *TP_NOTEQ = "NOTEQ";
char *TP_LOGAND = "LOGAND";
char *TP_LOGOR = "LOGOR";
char *TP_ELLIPSIS = "ELLIPSIS";
char *TP_TIMESEQ = "TIMESEQ";
char *TP_DIVEQ = "DIVEQ";
char *TP_MODEQ = "MODEQ";
char *TP_PLUSEQ = "PLUSEQ";
char *TP_MINUSEQ = "MINUSEQ";
char *TP_SHLEQ = "SHLEQ";
char *TP_SHREQ = "SHREQ";
char *TP_ANDEQ = "ANDEQ";
char *TP_OREQ = "OREQ";
char *TP_XOREQ = "XOREQ";
char *TP_AUTO = "AUTO";
char *TP_BREAK = "BREAK";
char *TP_CASE = "CASE";
char *TP_CHAR = "CHAR";
char *TP_CONST = "CONST";
char *TP_CONTINUE = "CONTINUE";
char *TP_DEFAULT = "DEFAULT";
char *TP_DO = "DO";
char *TP_DOUBLE = "DOUBLE";
char *TP_ELSE = "ELSE";
char *TP_ENUM = "ENUM";
char *TP_EXTERN = "EXTERN";
char *TP_FLOAT = "FLOAT";
char *TP_FOR = "FOR";
char *TP_GOTO = "GOTO";
char *TP_IF = "IF";
char *TP_INLINE = "INLINE";
char *TP_INT = "INT";
char *TP_LONG = "LONG";
char *TP_REGISTER = "REGISTER";
char *TP_RESTRICT = "RESTRICT";
char *TP_RETURN = "RETURN";
char *TP_SHORT = "SHORT";
char *TP_SIGNED = "SIGNED";
char *TP_SIZEOF = "SIZEOF";
char *TP_STATIC = "STATIC";
char *TP_STRUCT = "STRUCT";
char *TP_SWITCH = "SWITCH";
char *TP_TYPEDEF = "TYPEDEF";
char *TP_UNION = "UNION";
char *TP_UNSIGNED = "UNSIGNED";
char *TP_VOID = "VOID";
char *TP_VOLATILE = "VOLATILE";
char *TP_WHILE = "WHILE";
char *TP__BOOL = "_BOOL";
char *TP__COMPLEX = "_COMPLEX";
char *TP__IMAGINARY = "_IMAGINARY";
void print_escaped_char(char toprint);

char *nameof(yytoken_kind_t tok) {
  switch (tok) {
  case YYEOF:
    return TP_YYEOF;
  case IDENT:
    return TP_IDENT;
  case CHARLIT:
    return TP_CHARLIT;
  case STRING:
    return TP_STRING;
  case NUMBER:
    return TP_NUMBER;
  case INDSEL:
    return TP_INDSEL;
  case PLUSPLUS:
    return TP_PLUSPLUS;
  case MINUSMINUS:
    return TP_MINUSMINUS;
  case SHL:
    return TP_SHL;
  case SHR:
    return TP_SHR;
  case LTEQ:
    return TP_LTEQ;
  case GTEQ:
    return TP_GTEQ;
  case EQEQ:
    return TP_EQEQ;
  case NOTEQ:
    return TP_NOTEQ;
  case LOGAND:
    return TP_LOGAND;
  case LOGOR:
    return TP_LOGOR;
  case ELLIPSIS:
    return TP_ELLIPSIS;
  case TIMESEQ:
    return TP_TIMESEQ;
  case DIVEQ:
    return TP_DIVEQ;
  case MODEQ:
    return TP_MODEQ;
  case PLUSEQ:
    return TP_PLUSEQ;
  case MINUSEQ:
    return TP_MINUSEQ;
  case SHLEQ:
    return TP_SHLEQ;
  case SHREQ:
    return TP_SHREQ;
  case ANDEQ:
    return TP_ANDEQ;
  case OREQ:
    return TP_OREQ;
  case XOREQ:
    return TP_XOREQ;
  case AUTO:
    return TP_AUTO;
  case BREAK:
    return TP_BREAK;
  case CASE:
    return TP_CASE;
  case CHAR:
    return TP_CHAR;
  case CONST:
    return TP_CONST;
  case CONTINUE:
    return TP_CONTINUE;
  case DEFAULT:
    return TP_DEFAULT;
  case DO:
    return TP_DO;
  case DOUBLE:
    return TP_DOUBLE;
  case ELSE:
    return TP_ELSE;
  case ENUM:
    return TP_ENUM;
  case EXTERN:
    return TP_EXTERN;
  case FLOAT:
    return TP_FLOAT;
  case FOR:
    return TP_FOR;
  case GOTO:
    return TP_GOTO;
  case IF:
    return TP_IF;
  case INLINE:
    return TP_INLINE;
  case INT:
    return TP_INT;
  case LONG:
    return TP_LONG;
  case REGISTER:
    return TP_REGISTER;
  case RESTRICT:
    return TP_RESTRICT;
  case RETURN:
    return TP_RETURN;
  case SHORT:
    return TP_SHORT;
  case SIGNED:
    return TP_SIGNED;
  case SIZEOF:
    return TP_SIZEOF;
  case STATIC:
    return TP_STATIC;
  case STRUCT:
    return TP_STRUCT;
  case SWITCH:
    return TP_SWITCH;
  case TYPEDEF:
    return TP_TYPEDEF;
  case UNION:
    return TP_UNION;
  case UNSIGNED:
    return TP_UNSIGNED;
  case VOID:
    return TP_VOID;
  case VOLATILE:
    return TP_VOLATILE;
  case WHILE:
    return TP_WHILE;
  case _BOOL:
    return TP__BOOL;
  case _COMPLEX:
    return TP__COMPLEX;
  case _IMAGINARY:
    return TP__IMAGINARY;
  }
  sprintf(buf, "%c", tok);
  return buf;
}

void print_escaped_string(char *str, size_t len) {
  for (int i = 0; i < len; i++) {
    print_escaped_char(str[i]);
  }
}

void print_escaped_char(char toprint) {
  switch (toprint) {
  case '\'':
    printf("\\'");
    return;
  case '"':
    printf("\\\"");
    return;
  case '\\':
    printf("\\\\");
    return;
  }

  if (isprint(toprint)) {
    printf("%c", toprint);
    return;
  }

  switch (toprint) {
  case '\0':
    printf("\\0");
    break;
  case '\a':
    printf("\\a");
    break;
  case '\b':
    printf("\\b");
    break;
  case '\f':
    printf("\\f");
    break;
  case '\n':
    printf("\\n");
    break;
  case '\r':
    printf("\\r");
    break;
  case '\t':
    printf("\\t");
    break;
  case '\v':
    printf("\\v");
    break;
 default:
    printf("\\%03o", (unsigned char) toprint);
  }
}

void print_num() {
  switch (yylval.data.number.type) {
  case YYNUM_TYPE_FLOAT:
    if (yylval.data.number.is_unsigned)
      printf("REAL\t%fu\tUNSIGNED,FLOAT", yylval.data.number.num.yfloat);
    else
      printf("REAL\t%f\tFLOAT", yylval.data.number.num.yfloat);
    break;
  case YYNUM_TYPE_LONG_DOUBLE:
    if (yylval.data.number.is_unsigned)
      printf("REAL\t%LFu\tUNSIGNED,LONGDOUBLE",
             yylval.data.number.num.ylongdouble);
    else
      printf("REAL\t%LF\tLONGDOUBLE", yylval.data.number.num.ylongdouble);
    break;
  case YYNUM_TYPE_LONG:
    if (yylval.data.number.is_unsigned)
      printf("INTEGER\t%ld\tUNSIGNED,LONG", yylval.data.number.num.ylong);
    else
      printf("INTEGER\t%ld\tLONG", yylval.data.number.num.ylong);
    break;
  case YYNUM_TYPE_INT:
    if (yylval.data.number.is_unsigned)
      printf("INTEGER\t%d\tUNSIGNED,INT", yylval.data.number.num.yint);
    else
      printf("INTEGER\t%d\tINT", yylval.data.number.num.yint);
    break;
  case YYNUM_TYPE_LONGLONG:
    if (yylval.data.number.is_unsigned)
      printf("INTEGER\t%lld\tUNSIGNED,LONGLONG",
             yylval.data.number.num.ylonglong);
    else
      printf("INTEGER\t%lld\tLONGLONG", yylval.data.number.num.ylonglong);
    break;
  case YYNUM_TYPE_DOUBLE:
    if (yylval.data.number.is_unsigned)
      printf("REAL\t%lfu\tUNSIGNED,DOUBLE", yylval.data.number.num.ydouble);
    else
      printf("REAL\t%lf\tDOUBLE", yylval.data.number.num.ydouble);
    break;
  }
}

int main(int argc, char **argv) {
  while (true) {
    int val = yylex();
    if (val == 0)
      break;

    printf("%s\t%d\t%s\t", YY_FILENAME, yylineno, nameof(val));

    if (val == IDENT)
      printf("%s", yytext);

    if (val == NUMBER)
      print_num();

    if (val == STRING)
      print_escaped_string(yylval.data.string.string, yylval.data.string.length);

    if (val == CHARLIT)
      print_escaped_char(yylval.data.character);

    printf("\n");
  }
}

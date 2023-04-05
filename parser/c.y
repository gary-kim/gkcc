%{

%}

%token IDENT
%token CHARLIT
%token STRING
%token NUMBER
%token INDSEL
%token PLUSPLUS
%token MINUSMINUS
%token SHL
%token SHR
%token LTEQ
%token GTEQ
%token EQEQ
%token NOTEQ
%token LOGAND
%token LOGOR
%token ELLIPSIS
%token TIMESEQ
%token DIVEQ
%token MODEQ
%token PLUSEQ
%token MINUSEQ
%token SHLEQ
%token SHREQ
%token ANDEQ
%token OREQ
%token XOREQ
%token AUTO
%token BREAK
%token CASE
%token CHAR
%token CONST
%token CONTINUE
%token DEFAULT
%token DO
%token DOUBLE
%token ELSE
%token ENUM
%token EXTERN
%token FLOAT
%token FOR
%token GOTO
%token IF
%token INLINE
%token INT
%token LONG
%token REGISTER
%token RESTRICT
%token RETURN
%token SHORT
%token SIGNED
%token SIZEOF
%token STATIC
%token STRUCT
%token SWITCH
%token TYPEDEF
%token UNION
%token UNSIGNED
%token VOID
%token VOLATILE
%token WHILE
%token _BOOL
%token _COMPLEX
%token _IMAGINARY

/* NOT IMPLEMENTED LEFT */

%token _ALIGNAS
%token _ALIGNOF
%token _ATOMIC
%token _GENERIC
%token _NORETURN
%token _STATIC_ASSERT
%token _THREAD_LOCAL

%%

keyword: AUTO
       | BREAK
       | CASE
       | CHAR
       | CONST
       | CONTINUE
       | DEFAULT
       | DO
       | DOUBLE
       | ELSE
       | ENUM
       | EXTERN
       | FLOAT
       | FOR
       | GOTO
       | IF
       | INLINE
       | INT
       | LONG
       | REGISTER
       | RESTRICT
       | RETURN
       | SHORT
       | SIGNED
       | SIZEOF
       | STATIC
       | STRUCT
       | SWITCH
       | TYPEDEF
       | UNION
       | UNSIGNED
       | VOID
       | VOLATILE
       | WHILE
       | _ALIGNAS
       | _ALIGNOF
       | _ATOMIC
       | _BOOL
       | _COMPLEX
       | _GENERIC
       | _IMAGINARY
       | _NORETURN
       | _STATIC_ASSERT
       | _THREAD_LOCAL
       ;

punctuator: '['
          | ']'
          | '('
          | ')'
          | '{'
          | '}'
          | '.'
          | INDSEL
          | PLUSPLUS
          | MINUSMINUS
          | '&'
          | '*'
          | '+' | '-'
          | '~'
          | '!'
          | '/'
          | '%'
          | SHR
          | SHL
          | '<'
          | '>'
          | LTEQ
          | GTEQ
          | EQEQ
          | NOTEQ
          | '^'
          | '|'
          | LOGAND
          | LOGOR
          | '?'
          | ':'
          | ';'
          | ELLIPSIS
          | '='
          | TIMESEQ
          | DIVEQ
          | MODEQ
          | PLUSEQ
          | MINUSEQ
          | SHLEQ
          | SHREQ
          | ANDEQ
          | XOREQ
          | OREQ
          | ','
          ;

primary-expresson: IDENT
                 | constant
                 | STRING
                 | '(' expression ')'
                 ;

constant: NUMBER
        | CHARLIT
        ;

postfix-expression: primary-expression
                  | postfix-expression '[' expression ']'
                  | postfix-expression '(' ')'
                  | postfix-expression '(' argument-expression-list ')'
                  | postfix-expression '.' IDENT
                  | postfix-expression INDSEL  IDENT
                  | postfix-expression PLUSPLUS
                  | postfix-expression MINUSMINUS
                  | '(' type-name ')' '{' initializer-list '}'
                  | '(' type-name ')' '{' initializer-list ',' '}'
                  ;

argument-expression-list: /* empty */
                        | assignment-expression
                        | argument-expression-list ',' assignment-expression
                        ;

unary-expression: postfix-expression
                | PLUSPLUS unary-expression
                | MINUSMINUS unary-expression
                | unary-operator cast-expression
                | sizeof unary-expression
                | sizeof '(' type-name ')'
                ;

unary-operator: '&'
              | '*'
              | '+'
              | '-'
              | '~'
              | '!'
              ;

cast-expression: unary-expression
               | '(' type-name ')' cast-expression
               ;

multiplicative-expression: cast-expression
                         | multiplicative-expression '*' cast-expression
                         | multiplicative-expression '/' cast-expression
                         | multiplicative-expression '%' cast-expression
                         ;

additive-expression: multiplicative-expression
                   | additive-expression '+' multiplicative-expression
                   | additive-expression '-' multiplicative-expression
                   ;

shift-expression: additive-expression
                | shift-expression SHL additive-expression
                | shift-expression SHR additive-expression
                ;

relational-expression: shift-expression
                     | relational-expression '<' shift-expression
                     | relational-expression '>' shift-expression
                     | relational-expression LTEQ shift-expression
                     | relational-expression GTEQ shift-expression
                     ;

equality-expression: relational-expression
                   | equality-expression EQEQ relational-expression
                   | equality-expression NOTEQ relational-expression
                   ;

AND-expression: equality-expression
              | AND-expression '&' equality-expression
              ;

exclusive-OR-expression: AND-expression
                       | exclusive-OR-expression '^' AND-expression
                       ;

inclusive-OR-expression: exclusive-OR-expression
                       | inclusive-OR-expression '|' exclusive-OR-expression
                       ;

logical-AND-expression: inclusive-OR-expression
                      | logical-AND-expression LOGAND inclusive-OR-expression
                      ;

logical-OR-expression: logical-AND-expression
                     | logical-OR-expression LOGOR logical-AND-expression
                     ;

conditional-expression: logical-OR-expression
                      | logical-OR-expression '?' expression ':' conditional-expression
                      ;

assignment-expression: conditional-expression
                     | unary-expression assignment-operator assignment-expression
                     ;

assignment-operator: '='
                   | TIMESEQ
                   | DIVEQ
                   | MODEQ
                   | PLUSEQ
                   | MINUSEQ
                   | SHLEQ
                   | SHREQ
                   | ANDEQ
                   | XOREQ
                   | OREQ
                   ;

expression: assignment-expression
          | expression ',' assignment-expression
          ;

constant-expression: conditional-expression;


declaration: delcaration-specifiers init-declarator-list ';'
           | static_assert-delcartion
           ;

declaration-specifiers: /* empty */
                      | storage-class-specifier declarations-specifiers
                      | type-specifier declaration-specifiers
                      | type-specifier declaration-specifiers
                      | function-specifier declaration-specifiers
                      | alignment-specifier declaration-specifiers
                      ;

init-delcarator-list: init-declarator
                    | init-declarator-list ',' init-declarator
                    ;

init-declarator: declorator
               | declorator '=' initializer
               ;

storage-class-specifier: TYPEDEF
                       | EXTERN
                       | STATIC
                       | _THREAD_LOCAL
                       | AUTO
                       | REGISTER
                       ;

type-specifier: VOID
              | CHAR
              | SHORT
              | INT
              | LONG
              | FLOAT
              | DOUBLE
              | SIGNED
              | UNSIGNED
              | _BOOL
              | _COMPLEX
              | atomic-type-specifier
              | struct-or-union-specifier
              | enum-specifier
              | typedef-name
              ;

struct-or-union-specifier: struct-or-union identifier '{' struct-declaration-list '}'
                         | struct-or-union '{' struct-declaration-list '}'
                         | struct-or-union identifier
                         ;

struct-or-union: STRUCT
               | UNION
               ;

struct-declaration-list: /* empty */
                       | struct-declaration
                       | struct-declaration-list struct-declaration
                       ;

struct-declaration: struct-qualifier-list struct-declarator-list
                  | static_assert_declaration
                  ;

specifier-qualifier-list: type-specifier
                        | type-specifier specifier-qualifier-list
                        | type-qualifier
                        | type-qualifier specifier-qualifier-list
                        ;

struct-declarator-list: struct-declarator
                      | struct-declarator-list ',' struct-declarator
                      ;

struct-declarator: declarator
                 | declarator ':' constant-expression
                 | ':' constant-expression
                 ;

enum-specifier: ENUM '{' enumerator-list '}'
              | ENUM identifier '{' enumerator-list '}'
              | ENUM '{' enumerator-list ',' '}'
              | ENUM identifier '{' enumerator-list ',' '}'
              | ENUM identifier
              ;

enumerator-list: enumerator
               | enumerator-list ',' enumerator
               ;

enumerator: enumeration-constant
          | enumeration-constant ',' enumerator;


atomic-type-specifier: _ATOMIC '(' type-name ')';

type-qualifier: CONST
              | RESTRICT
              | VOLATILE
              | _ATOMIC
              ;

function-specifier: INLINE
                  | _NORETURN
                  ;

alignment-operator: _ALIGNAS '(' type-name ')'
                  | _ALIGNAS '(' constant-expression ')'
                  ;

declarator: direct-declarator
          | pointer direct-declarator
          ;




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
%token _GENERIC
%token _NORETURN
%token _STATIC_ASSERT
%token _THREAD_LOCAL

%left IF
%left ELSE

%%

declaration_or_fndef: declaration
                    | function-definition
                    ;

primary-expression: IDENT
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

argument-expression-list: assignment-expression
                        | argument-expression-list ',' assignment-expression
                        ;

unary-expression: postfix-expression
                | PLUSPLUS unary-expression
                | MINUSMINUS unary-expression
                | unary-operator cast-expression
                | SIZEOF unary-expression
                | SIZEOF '(' type-name ')'
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


declaration: declaration-specifiers ';'
           | declaration-specifiers init-declarator-list ';'
           //| static_assert-declaration // NOT IMPLEMENTED
           ;

declaration-specifiers: storage-class-specifier
                      | declaration-specifiers storage-class-specifier
                      | type-specifier
                      | declaration-specifiers type-specifier
                      | type-qualifier
                      | declaration-specifiers type-qualifier
                      | function-specifier
                      | declaration-specifiers function-specifier
                      | alignment-specifier
                      | declaration-specifiers alignment-specifier
                      ;

init-declarator-list: init-declarator
                    | init-declarator-list ',' init-declarator
                    ;

init-declarator: declarator
               | declarator '=' initializer
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
              | struct-or-union-specifier
              | enum-specifier
              //| typedef-name // NOT IMPLEMENTED
              //;

struct-or-union-specifier: struct-or-union IDENT '{' struct-declaration-list '}'
                         | struct-or-union '{' struct-declaration-list '}'
                         | struct-or-union IDENT
                         ;

struct-or-union: STRUCT
               | UNION
               ;

struct-declaration-list: struct-declaration
                       | struct-declaration-list struct-declaration
                       ;

struct-declaration: specifier-qualifier-list ';'
                  | specifier-qualifier-list struct-declarator-list ';'
                  // | static_assert-declaration // NOT IMPLEMENTED
                  ;

specifier-qualifier-list: type-specifier
                        | specifier-qualifier-list type-specifier
                        | type-qualifier
                        | specifier-qualifier-list type-qualifier
                        ;

struct-declarator-list: struct-declarator
                      | struct-declarator-list ',' struct-declarator
                      ;

struct-declarator: declarator
                 | declarator ':' constant-expression
                 | ':' constant-expression
                 ;

enum-specifier: ENUM '{' enumerator-list '}'
              | ENUM IDENT '{' enumerator-list '}'
              | ENUM '{' enumerator-list ',' '}'
              | ENUM IDENT '{' enumerator-list ',' '}'
              | ENUM IDENT
              ;

enumerator-list: enumerator
               | enumerator-list ',' enumerator
               ;

//enumeration-constant: IDENT;

enumerator: IDENT
          | IDENT '=' constant-expression
          ;


type-qualifier: CONST
              | RESTRICT
              | VOLATILE
              ;

function-specifier: INLINE
                  | _NORETURN
                  ;

alignment-specifier: _ALIGNAS '(' type-name ')'
                  | _ALIGNAS '(' constant-expression ')'
                  ;

declarator: direct-declarator
          | pointer direct-declarator
          ;

direct-declarator: IDENT
                 | '(' declarator ')'
                 | direct-declarator '[' ']'
                 | direct-declarator '[' type-qualifier-list ']'
                 | direct-declarator '[' assignment-expression ']'
                 | direct-declarator '[' type-qualifier-list assignment-expression']'
                 | direct-declarator '[' STATIC assignment-expression ']'
                 | direct-declarator '[' STATIC type-qualifier-list assignment-expression ']'
                 | direct-declarator '[' type-qualifier-list STATIC assignment-expression ']'
                 | direct-declarator '[' type-qualifier-list '*' ']'
                 | direct-declarator '[' '*' ']'
                 | direct-declarator '(' parameter-type-list ')'
                 | direct-declarator '(' identifier-list ')'
                 | direct-declarator '(' ')'
                 ;

pointer: '*'
       | '*' type-qualifier-list
       | '*' type-qualifier-list pointer
       | '*' pointer
       ;

type-qualifier-list: type-qualifier
                   | type-qualifier-list type-qualifier
                   ;

parameter-type-list: parameter-list
                   | parameter-list ',' ELLIPSIS
                   ;

parameter-list: parameter-declaration
              | parameter-list ',' parameter-declaration
              ;

parameter-declaration: declaration-specifiers declarator
                     | declaration-specifiers
                     //| declaration-specifiers abstract-declarator // NOT IMPLEMENTED
                     ;

identifier-list: IDENT
               | identifier-list ',' IDENT
               ;

type-name: specifier-qualifier-list
         // | specifier-qualifier-list abstract-declarator // NOT IMPLEMENTED
         ;

// abstract-declarator not implemented
// direct-abstract-declarator not implemented
// typedef-name not implemented

initializer: assignment-expression
           | '{' initializer-list '}'
           | '{' initializer-list ',' '}'
           ;

initializer-list: initializer
                | designation initializer
                | initializer-list ',' initializer
                | initializer-list ',' designation initializer
                ;

designation: designator-list '=';

designator-list: designator
               | designator-list designator
               ;

designator: '[' constant-expression ']'
          | '.' IDENT
          ;

// static_assert-declaration: _STATIC_ASSERT '(' constant-expression ',' STRING ')' ';'; // NOT IMPLEMENTED

// === BEGIN STATEMENTS ===

compound-statement: '{' '}'
                  | '{' block-item-list '}'
                  ;


statement: labeled-statement
         | compound-statement
         | expression-statement
         | selection-statement
         | iteration-statement
         | jump-statement
         ;

labeled-statement: IDENT ':' statement
                 | CASE constant-expression ':' statement
                 | DEFAULT ':' statement
                 ;

block-item-list: block-item
               | block-item-list block-item
               ;

block-item: declaration
          | statement
          ;

expression-statement: ';'
                    | expression ';'
                    ;

selection-statement: IF '(' expression ')' statement %prec IF
                   | IF '(' expression ')' statement ELSE statement %prec ELSE
                   | SWITCH '(' expression ')' statement
                   ;

iteration-statement: WHILE '(' expression ')' statement
                   | DO statement WHILE '(' expression ')' ';'
                   | FOR '(' ';' ';' ')' statement
                   | FOR '(' expression ';' ';' ')' statement
                   | FOR '(' ';' expression ';' ')' statement
                   | FOR '(' expression ';' expression ';' ')' statement
                   | FOR '(' ';' ';' expression ')' statement
                   | FOR '(' expression ';' ';' expression ')' statement
                   | FOR '(' expression ';' expression ';' expression ')' statement
                   | FOR '(' declaration ';' ')'
                   | FOR '(' declaration expression ';' ')'
                   | FOR '(' declaration ';' expression ')'
                   | FOR '(' declaration expression ';' expression ')'
                   ;

jump-statement: GOTO IDENT ';'
              | CONTINUE ';'
              | BREAK ';'
              | RETURN ';'
              | RETURN expression ';'
              ;

// === BEGIN EXTERNAL DEFINITIONS ===

function-definition: declaration-specifiers declarator compound-statement
                   | declaration-specifiers declarator declaration-list compound-statement
                   ;

declaration-list: declaration
                | declaration-list declaration
                ;


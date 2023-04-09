%{
#include "lex_extras.h"
#include "ast.h"
#include "scope.h"

#include <stdio.h>

static void yyerror() {
  fprintf(stderr, "ERROR\n");
}
%}

%parse-param { struct ast_node* top_ast_node }
%parse-param { struct gkcc_symbol_table_set *current_symbol_table }

%union {
  struct _yylval yylval;
  struct ast_node* ast_node;
}

%token <yylval> IDENT
%token <yylval> CHARLIT
%token <yylval> STRING
%token <yylval> NUMBER
%token <yylval> INDSEL
%token <yylval> PLUSPLUS
%token <yylval> MINUSMINUS
%token <yylval> SHL
%token <yylval> SHR
%token <yylval> LTEQ
%token <yylval> GTEQ
%token <yylval> EQEQ
%token <yylval> NOTEQ
%token <yylval> LOGAND
%token <yylval> LOGOR
%token <yylval> ELLIPSIS
%token <yylval> TIMESEQ
%token <yylval> DIVEQ
%token <yylval> MODEQ
%token <yylval> PLUSEQ
%token <yylval> MINUSEQ
%token <yylval> SHLEQ
%token <yylval> SHREQ
%token <yylval> ANDEQ
%token <yylval> OREQ
%token <yylval> XOREQ
%token <yylval> AUTO
%token <yylval> BREAK
%token <yylval> CASE
%token <yylval> CHAR
%token <yylval> CONST
%token <yylval> CONTINUE
%token <yylval> DEFAULT
%token <yylval> DO
%token <yylval> DOUBLE
%token <yylval> ELSE
%token <yylval> ENUM
%token <yylval> EXTERN
%token <yylval> FLOAT
%token <yylval> FOR
%token <yylval> GOTO
%token <yylval> IF
%token <yylval> INLINE
%token <yylval> INT
%token <yylval> LONG
%token <yylval> REGISTER
%token <yylval> RESTRICT
%token <yylval> RETURN
%token <yylval> SHORT
%token <yylval> SIGNED
%token <yylval> SIZEOF
%token <yylval> STATIC
%token <yylval> STRUCT
%token <yylval> SWITCH
%token <yylval> TYPEDEF
%token <yylval> UNION
%token <yylval> UNSIGNED
%token <yylval> VOID
%token <yylval> VOLATILE
%token <yylval> WHILE
%token <yylval> _BOOL
%token <yylval> _COMPLEX
%token <yylval> _IMAGINARY

/* NOT IMPLEMENTED LEFT */

%token <yylval> _ALIGNAS
%token <yylval> _ALIGNOF
%token <yylval> _GENERIC
%token <yylval> _NORETURN
%token <yylval> _STATIC_ASSERT
%token <yylval> _THREAD_LOCAL

%left IF
%left ELSE

%nterm <ast_node> declaration_or_fndef
%nterm <ast_node> primary-expression
%nterm <ast_node> constant
%nterm <ast_node> postfix-expression
%nterm <ast_node> argument-expression-list
%nterm <ast_node> unary-expression
%nterm <ast_node> unary-operator
%nterm <ast_node> cast-expression
%nterm <ast_node> multiplicative-expression
%nterm <ast_node> additive-expression
%nterm <ast_node> shift-expression
%nterm <ast_node> relational-expression
%nterm <ast_node> equality-expression
%nterm <ast_node> AND-expression
%nterm <ast_node> exclusive-OR-expression
%nterm <ast_node> inclusive-OR-expression
%nterm <ast_node> logical-AND-expression
%nterm <ast_node> logical-OR-expression
%nterm <ast_node> conditional-expression
%nterm <ast_node> assignment-expression
%nterm <ast_node> assignment-operator
%nterm <ast_node> expression
%nterm <ast_node> constant-expression
%nterm <ast_node> declaration
%nterm <ast_node> declaration-specifiers
%nterm <ast_node> init-declarator-list
%nterm <ast_node> init-declarator
%nterm <ast_node> storage-class-specifier
%nterm <ast_node> type-specifier
%nterm <ast_node> struct-or-union-specifier
%nterm <ast_node> struct-or-union
%nterm <ast_node> struct-declaration-list
%nterm <ast_node> struct-declaration
%nterm <ast_node> specifier-qualifier-list
%nterm <ast_node> struct-declarator-list
%nterm <ast_node> struct-declarator
%nterm <ast_node> enum-specifier
%nterm <ast_node> enumerator-list
%nterm <ast_node> enumerator
%nterm <ast_node> type-qualifier
%nterm <ast_node> function-specifier
%nterm <ast_node> alignment-specifier
%nterm <ast_node> declarator
%nterm <ast_node> direct-declarator
%nterm <ast_node> pointer
%nterm <ast_node> type-qualifier-list
%nterm <ast_node> parameter-type-list
%nterm <ast_node> parameter-list
%nterm <ast_node> parameter-declaration
%nterm <ast_node> identifier-list
%nterm <ast_node> type-name
%nterm <ast_node> initializer
%nterm <ast_node> initializer-list
%nterm <ast_node> designation
%nterm <ast_node> designator-list
%nterm <ast_node> designator
%nterm <ast_node> compound-statement
%nterm <ast_node> statement
%nterm <ast_node> labeled-statement
%nterm <ast_node> block-item-list
%nterm <ast_node> block-item
%nterm <ast_node> expression-statement
%nterm <ast_node> selection-statement
%nterm <ast_node> iteration-statement
%nterm <ast_node> jump-statement
%nterm <ast_node> function-definition
%nterm <ast_node> declaration-list

%%

declaration_or_fndef: declaration {
                        *top_ast_node = *$1;
                      }
                    | function-definition {
                        *top_ast_node = *$1;
                      }
                    | expression {
                        *top_ast_node = *$1;
                      }
                    ;

primary-expression: IDENT
                 | constant
                 | STRING
                 | '(' expression ')' {
                     $$ = $expression;
                   }
                 ;

constant: NUMBER {
            $$ = yylval2ast_node(&$NUMBER);
          }
        | CHARLIT {
            $$ = yylval2ast_node(&$CHARLIT);
          }
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

// 6.5.3
unary-expression: postfix-expression
                | PLUSPLUS unary-expression {
                    // Equivalent
                    struct ast_node *constant_1 = ast_node_new_constant_int_node(1);
                    $$ = ast_node_new_binop_node(AST_BINOP_ASSIGN_ADD, $2, constant_1);
                  }
                | MINUSMINUS unary-expression {
                    // Equivalent
                    struct ast_node *constant_1 = ast_node_new_constant_int_node(1);
                    $$ = ast_node_new_binop_node(AST_BINOP_ASSIGN_SUBTRACT, $2, constant_1);
                  }
                | '&' cast-expression {
                    $$ = ast_node_new_unary_node(AST_UNARY_ADDRESSOF, $2);
                  }
                | '*' cast-expression {
                    $$ = ast_node_new_unary_node(AST_UNARY_DEREFERENCE, $2);
                  }
                | '+' cast-expression {
                    $$ = $2;
                  }
                | '-' cast-expression {
                    $$ = ast_node_new_unary_node(AST_UNARY_NEGATIVE, $2);
                  }
                | '~' cast-expression {
                    $$ = ast_node_new_unary_node(AST_UNARY_BITWISE_NOT, $2);
                  }
                | '!' cast-expression {
                    $$ = ast_node_new_unary_node(AST_UNARY_LOGICAL_NOT, $2);
                  }
                | SIZEOF unary-expression {
                    $$ = ast_node_new_unary_node(AST_UNARY_SIZEOF, $2);
                  }
                | SIZEOF '(' type-name ')'
                ;

cast-expression: unary-expression
               | '(' type-name ')' cast-expression
               ;

multiplicative-expression: cast-expression
                         | multiplicative-expression '*' cast-expression {
                             $$ = ast_node_new_binop_node(AST_BINOP_MULTIPLY, $1, $3);
                           }
                         | multiplicative-expression '/' cast-expression {
                             $$ = ast_node_new_binop_node(AST_BINOP_DIVIDE, $1, $3);
                           }
                         | multiplicative-expression '%' cast-expression {
                             $$ = ast_node_new_binop_node(AST_BINOP_MOD, $1, $3);
                           }
                         ;

additive-expression: multiplicative-expression
                   | additive-expression '+' multiplicative-expression {
                       $$ = ast_node_new_binop_node(AST_BINOP_ADD, $1, $3);
                     }
                   | additive-expression '-' multiplicative-expression {
                       $$ = ast_node_new_binop_node(AST_BINOP_SUBTRACT, $1, $3);
                     }
                   ;

// 6.5.7
shift-expression: additive-expression
                | shift-expression SHL additive-expression {
                    $$ = ast_node_new_binop_node(AST_BINOP_SHL, $1, $3);
                  }
                | shift-expression SHR additive-expression {
                    $$ = ast_node_new_binop_node(AST_BINOP_SHR, $1, $3);
                  }
                ;

relational-expression: shift-expression
                     | relational-expression '<' shift-expression {
                         $$ = ast_node_new_binop_node(AST_BINOP_LT, $1, $3);
                       }
                     | relational-expression '>' shift-expression {
                         $$ = ast_node_new_binop_node(AST_BINOP_GT, $1, $3);
                       }
                     | relational-expression LTEQ shift-expression {
                         $$ = ast_node_new_binop_node(AST_BINOP_LTEQ, $1, $3);
                       }
                     | relational-expression GTEQ shift-expression {
                         $$ = ast_node_new_binop_node(AST_BINOP_GTEQ, $1, $3);
                       }
                     ;

equality-expression: relational-expression
                   | equality-expression EQEQ relational-expression {
                       $$ = ast_node_new_binop_node(AST_BINOP_EQUALS, $1, $3);
                     }
                   | equality-expression NOTEQ relational-expression {
                       struct ast_node *eq_node = ast_node_new_binop_node(AST_BINOP_EQUALS, $1, $3);
                       $$ = ast_node_new_unary_node(AST_UNARY_NOT, eq_node);
                     }
                   ;

AND-expression: equality-expression
              | AND-expression '&' equality-expression {
                  $$ = ast_node_new_binop_node(AST_BINOP_BITWISE_AND, $1, $3);
                }
              ;

exclusive-OR-expression: AND-expression
                       | exclusive-OR-expression '^' AND-expression {
                           $$ = ast_node_new_binop_node(AST_BINOP_BITWISE_XOR, $1, $3);
                         }
                       ;

inclusive-OR-expression: exclusive-OR-expression
                       | inclusive-OR-expression '|' exclusive-OR-expression {
                           $$ = ast_node_new_binop_node(AST_BINOP_BITWISE_OR, $1, $3);
                         }
                       ;

logical-AND-expression: inclusive-OR-expression
                      | logical-AND-expression LOGAND inclusive-OR-expression {
                          $$ = ast_node_new_binop_node(AST_BINOP_BITWISE_AND, $1, $3);
                        }
                      ;

logical-OR-expression: logical-AND-expression
                     | logical-OR-expression LOGOR logical-AND-expression {
                         $$ = ast_node_new_binop_node(AST_BINOP_LOGICAL_OR, $1, $3);
                       }
                     ;

conditional-expression: logical-OR-expression
                      | logical-OR-expression '?' expression ':' conditional-expression {
                          $$ = ast_node_new_ternary_node($1, $3, $5);
                        }
                      ;

assignment-expression: conditional-expression
                     | unary-expression assignment-operator assignment-expression {
                         struct ast_node *n = $2;
                         n->binop.left = $1;
                         n->binop.right = $3;
                         $$ = n;
                       }
                     ;

assignment-operator: '=' {
                       $$ = ast_node_new_binop_node(AST_BINOP_ASSIGN, NULL, NULL);
                     }
                   | TIMESEQ {
                       $$ = ast_node_new_binop_node(AST_BINOP_ASSIGN_MULTIPLY, NULL, NULL);
                     }
                   | DIVEQ {
                       $$ = ast_node_new_binop_node(AST_BINOP_ASSIGN_DIVIDE, NULL, NULL);
                     }
                   | MODEQ {
                       $$ = ast_node_new_binop_node(AST_BINOP_ASSIGN_MOD, NULL, NULL);
                     }
                   | PLUSEQ {
                        $$ = ast_node_new_binop_node(AST_BINOP_ASSIGN_ADD, NULL, NULL);
                      }
                   | MINUSEQ {
                         $$ = ast_node_new_binop_node(AST_BINOP_ASSIGN_SUBTRACT, NULL, NULL);
                       }
                   | SHLEQ {
                       $$ = ast_node_new_binop_node(AST_BINOP_ASSIGN_BITWISE_SHL, NULL, NULL);
                     }
                   | SHREQ {
                       $$ = ast_node_new_binop_node(AST_BINOP_ASSIGN_BITWISE_SHR, NULL, NULL);
                     }
                   | ANDEQ {
                       $$ = ast_node_new_binop_node(AST_BINOP_ASSIGN_BITWISE_AND, NULL, NULL);
                     }
                   | XOREQ {
                       $$ = ast_node_new_binop_node(AST_BINOP_ASSIGN_BITWISE_XOR, NULL, NULL);
                     }
                   | OREQ {
                      $$ = ast_node_new_binop_node(AST_BINOP_ASSIGN_BITWISE_OR, NULL, NULL);
                    }
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


type-qualifier: CONST {
                  $$ = ast_node_new_gkcc_type_qualifier_node(GKCC_QUALIFIER_CONST, NULL);
                }
              | RESTRICT {
                  $$ = ast_node_new_gkcc_type_qualifier_node(GKCC_QUALIFIER_RESTRICT, NULL);
                }
              | VOLATILE {
                  $$ = ast_node_new_gkcc_type_qualifier_node(GKCC_QUALIFIER_VOLATILE, NULL);
                }
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
                 | '(' declarator ')' {
                     $$ = $declarator;
                   }
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


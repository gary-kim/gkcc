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

%code requires {
#include "lex_extras.h"
#include "ast.h"
#include "ast_constructors.h"
#include "scope.h"
#include "scope_helpers.h"
}

%code {
#include <stdio.h>
#include "lex.yy.h"

static void yyerror() {
  gkcc_error_fatal(GKCC_ERROR_YYERROR, "Got yyerror");
}

#define ENTER_SCOPE(TYPE) \
  current_symbol_table = gkcc_symbol_table_set_new(current_symbol_table, TYPE)

#define EXIT_SCOPE() \
  current_symbol_table = gkcc_symbol_table_set_get_parent_symbol_table_set(current_symbol_table)

}

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

%precedence IF
%precedence ELSE

%nterm <ast_node> declaration_or_fndef
%nterm <ast_node> primary_expression
%nterm <ast_node> identifier
%nterm <ast_node> constant
%nterm <ast_node> postfix_expression
%nterm <ast_node> argument_expression_list
%nterm <ast_node> unary_expression
%nterm <ast_node> unary_operator
%nterm <ast_node> cast_expression
%nterm <ast_node> multiplicative_expression
%nterm <ast_node> additive_expression
%nterm <ast_node> shift_expression
%nterm <ast_node> relational_expression
%nterm <ast_node> equality_expression
%nterm <ast_node> AND_expression
%nterm <ast_node> exclusive_OR_expression
%nterm <ast_node> inclusive_OR_expression
%nterm <ast_node> logical_AND_expression
%nterm <ast_node> logical_OR_expression
%nterm <ast_node> conditional_expression
%nterm <ast_node> assignment_expression
%nterm <ast_node> assignment_operator
%nterm <ast_node> expression
%nterm <ast_node> constant_expression
%nterm <ast_node> declaration
%nterm <ast_node> declaration_specifiers
%nterm <ast_node> init_declarator_list
%nterm <ast_node> init_declarator
%nterm <ast_node> storage_class_specifier
%nterm <ast_node> type_specifier
%nterm <ast_node> struct_or_union_specifier
%nterm <ast_node> struct_or_union
%nterm <ast_node> struct_declaration_list
%nterm <ast_node> struct_declaration
%nterm <ast_node> specifier_qualifier_list
%nterm <ast_node> struct_declarator_list
%nterm <ast_node> struct_declarator
%nterm <ast_node> enum_specifier
%nterm <ast_node> enumerator_list
%nterm <ast_node> enumerator
%nterm <ast_node> type_qualifier
%nterm <ast_node> function_specifier
%nterm <ast_node> alignment_specifier
%nterm <ast_node> declarator
%nterm <ast_node> direct_declarator
%nterm <ast_node> pointer
%nterm <ast_node> type_qualifier_list
%nterm <ast_node> parameter_type_list
%nterm <ast_node> parameter_list
%nterm <ast_node> parameter_declaration
%nterm <ast_node> identifier_list
%nterm <ast_node> type_name
%nterm <ast_node> initializer
%nterm <ast_node> initializer_list
%nterm <ast_node> designation
%nterm <ast_node> designator_list
%nterm <ast_node> designator
%nterm <ast_node> compound_statement
%nterm <ast_node> statement
%nterm <ast_node> labeled_statement
%nterm <ast_node> block_item_list
%nterm <ast_node> block_item
%nterm <ast_node> expression_statement
%nterm <ast_node> selection_statement
%nterm <ast_node> iteration_statement
%nterm <ast_node> jump_statement
%nterm <ast_node> function_definition
%nterm <ast_node> declaration_list
%nterm <ast_node> top_list

%%

top_list: declaration_or_fndef {
            $$ = ast_node_new_list_node($1);
            *top_ast_node = *$$;
          }
        | top_list declaration_or_fndef {
            $$ = ast_node_append($1, $2);
            *top_ast_node = *$$;
          }
        ;

declaration_or_fndef: declaration
                    | function_definition
                    ;

identifier: IDENT {
  $$ = yylval2ast_node_ident(&$1);
};

primary_expression: identifier {
                      $$ = ast_node_identifier_set_symbol_if_exists(current_symbol_table, $identifier, GKCC_NAMESPACE_GENERAL);
                      ast_node_identifier_verify_symbol_exists($$, YY_FILENAME, yylineno);
                   }
                 | constant
                 | STRING {
                     $$ = yylval2ast_node(&$STRING);
                   }
                 | '(' expression ')' {
                     $$ = ast_node_strip_single_list($expression);
                   }
                 ;

constant: NUMBER {
            $$ = yylval2ast_node(&$NUMBER);
          }
        | CHARLIT {
            $$ = yylval2ast_node(&$CHARLIT);
          }
        ;

postfix_expression: primary_expression
                  | postfix_expression '[' expression ']' {
                      struct ast_node *new_pos = ast_node_new_binop_node(AST_BINOP_ADD, $1, ast_node_strip_single_list($expression));
                      $$ = ast_node_new_unary_node(AST_UNARY_DEREFERENCE, new_pos);
                    }
                  | postfix_expression '(' ')' {
                      $$ = ast_node_new_function_call_node($1, NULL);
                    }
                  | postfix_expression '(' argument_expression_list ')' {
                      $$ = ast_node_new_function_call_node($1, $argument_expression_list);
                    }
                  | postfix_expression '.' identifier {
                      $$ = ast_node_new_member_access_node(current_symbol_table, $1, $identifier);
                    }
                  | postfix_expression INDSEL identifier {
                      struct ast_node *deref = ast_node_new_unary_node(AST_UNARY_DEREFERENCE, $1);
                      $$ = ast_node_new_member_access_node(current_symbol_table, deref, $identifier);
                    }
                  | postfix_expression PLUSPLUS {
                      $$ = ast_node_new_unary_node(AST_UNARY_POSTINC, $1);
                    }
                  | postfix_expression MINUSMINUS {
                      $$ = ast_node_new_unary_node(AST_UNARY_POSTDEC, $1);
                    }
                  //| '(' type_name ')' '{' initializer_list '}'
                  //| '(' type_name ')' '{' initializer_list ',' '}'
                  // TODO: Figure out if it is important that these be implemented properly.
                  ;

argument_expression_list: assignment_expression {
                            $$ = ast_node_new_list_node($1);
                          }
                        | argument_expression_list ',' assignment_expression {
                            $$ = ast_node_append($1, $3);
                         }
                        ;

// 6.5.3
unary_expression: postfix_expression
                | PLUSPLUS unary_expression {
                    // Equivalent
                    struct ast_node *constant_1 = ast_node_new_constant_int_node(1);
                    $$ = ast_node_new_binop_node(AST_BINOP_ASSIGN_ADD, $2, constant_1);
                  }
                | MINUSMINUS unary_expression {
                    // Equivalent
                    struct ast_node *constant_1 = ast_node_new_constant_int_node(1);
                    $$ = ast_node_new_binop_node(AST_BINOP_ASSIGN_SUBTRACT, $2, constant_1);
                  }
                | '&' cast_expression {
                    $$ = ast_node_new_unary_node(AST_UNARY_ADDRESSOF, $2);
                  }
                | '*' cast_expression {
                    $$ = ast_node_new_unary_node(AST_UNARY_DEREFERENCE, $2);
                  }
                | '+' cast_expression {
                    $$ = $2;
                  }
                | '-' cast_expression {
                    $$ = ast_node_new_unary_node(AST_UNARY_NEGATIVE, $2);
                  }
                | '~' cast_expression {
                    $$ = ast_node_new_unary_node(AST_UNARY_BITWISE_NOT, $2);
                  }
                | '!' cast_expression {
                    $$ = ast_node_new_unary_node(AST_UNARY_LOGICAL_NOT, $2);
                  }
                | SIZEOF unary_expression {
                    $$ = ast_node_new_unary_node(AST_UNARY_SIZEOF, $2);
                  }
                | SIZEOF '(' type_name ')' {
                    $$ = ast_node_new_unary_node(AST_UNARY_SIZEOF, $type_name);
                  }
                ;

cast_expression: unary_expression
               | '(' type_name ')' cast_expression {
                   $$ = ast_node_new_binop_node(AST_BINOP_CAST, $2, $4);
                 }
               ;

multiplicative_expression: cast_expression
                         | multiplicative_expression '*' cast_expression {
                             $$ = ast_node_new_binop_node(AST_BINOP_MULTIPLY, $1, $3);
                           }
                         | multiplicative_expression '/' cast_expression {
                             $$ = ast_node_new_binop_node(AST_BINOP_DIVIDE, $1, $3);
                           }
                         | multiplicative_expression '%' cast_expression {
                             $$ = ast_node_new_binop_node(AST_BINOP_MOD, $1, $3);
                           }
                         ;

additive_expression: multiplicative_expression
                   | additive_expression '+' multiplicative_expression {
                       $$ = ast_node_new_binop_node(AST_BINOP_ADD, $1, $3);
                     }
                   | additive_expression '-' multiplicative_expression {
                       $$ = ast_node_new_binop_node(AST_BINOP_SUBTRACT, $1, $3);
                     }
                   ;

// 6.5.7
shift_expression: additive_expression
                | shift_expression SHL additive_expression {
                    $$ = ast_node_new_binop_node(AST_BINOP_SHL, $1, $3);
                  }
                | shift_expression SHR additive_expression {
                    $$ = ast_node_new_binop_node(AST_BINOP_SHR, $1, $3);
                  }
                ;

relational_expression: shift_expression
                     | relational_expression '<' shift_expression {
                         $$ = ast_node_new_binop_node(AST_BINOP_LT, $1, $3);
                       }
                     | relational_expression '>' shift_expression {
                         $$ = ast_node_new_binop_node(AST_BINOP_GT, $1, $3);
                       }
                     | relational_expression LTEQ shift_expression {
                         $$ = ast_node_new_binop_node(AST_BINOP_LTEQ, $1, $3);
                       }
                     | relational_expression GTEQ shift_expression {
                         $$ = ast_node_new_binop_node(AST_BINOP_GTEQ, $1, $3);
                       }
                     ;

equality_expression: relational_expression
                   | equality_expression EQEQ relational_expression {
                       $$ = ast_node_new_binop_node(AST_BINOP_EQUALS, $1, $3);
                     }
                   | equality_expression NOTEQ relational_expression {
                       struct ast_node *eq_node = ast_node_new_binop_node(AST_BINOP_EQUALS, $1, $3);
                       $$ = ast_node_new_unary_node(AST_UNARY_LOGICAL_NOT, eq_node);
                     }
                   ;

AND_expression: equality_expression
              | AND_expression '&' equality_expression {
                  $$ = ast_node_new_binop_node(AST_BINOP_BITWISE_AND, $1, $3);
                }
              ;

exclusive_OR_expression: AND_expression
                       | exclusive_OR_expression '^' AND_expression {
                           $$ = ast_node_new_binop_node(AST_BINOP_BITWISE_XOR, $1, $3);
                         }
                       ;

inclusive_OR_expression: exclusive_OR_expression
                       | inclusive_OR_expression '|' exclusive_OR_expression {
                           $$ = ast_node_new_binop_node(AST_BINOP_BITWISE_OR, $1, $3);
                         }
                       ;

logical_AND_expression: inclusive_OR_expression
                      | logical_AND_expression LOGAND inclusive_OR_expression {
                          $$ = ast_node_new_binop_node(AST_BINOP_LOGICAL_AND, $1, $3);
                        }
                      ;

logical_OR_expression: logical_AND_expression
                     | logical_OR_expression LOGOR logical_AND_expression {
                         $$ = ast_node_new_binop_node(AST_BINOP_LOGICAL_OR, $1, $3);
                       }
                     ;

conditional_expression: logical_OR_expression
                      | logical_OR_expression '?' expression ':' conditional_expression {
                          $$ = ast_node_new_ternary_node($1, $3, $5);
                        }
                      ;

assignment_expression: conditional_expression
                     | unary_expression assignment_operator assignment_expression {
                         struct ast_node *n = $2;
                         n->binop.left = $1;
                         n->binop.right = $3;
                         $$ = n;
                       }
                     ;

assignment_operator: '=' {
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

expression: assignment_expression {
              $$ = ast_node_new_list_node($1);
            }
          | expression ',' assignment_expression {
              $$ = ast_node_append($1, $3);
            }
          ;

constant_expression: conditional_expression;


declaration: declaration_specifiers ';' {
               $$ = ast_node_new_declaration_node($1, NULL);
             }
           | declaration_specifiers init_declarator_list ';' {
               $$ = ast_node_new_declaration_node($1, $2);
               gkcc_scope_add_variable_to_scope(current_symbol_table, $$, yylineno, YY_FILENAME);
           }
           //| static_assert_declaration // NOT IMPLEMENTED
           ;

declaration_specifiers: storage_class_specifier {
                          $$ = ast_node_new_list_node($1);
                        }
                      | declaration_specifiers storage_class_specifier {
                          $$ = ast_node_append($1, $2);
                        }
                      | type_specifier {
                          $$ = ast_node_new_list_node($1);
                        }
                      | declaration_specifiers type_specifier {
                          $$ = ast_node_append($1, $2);
                        }
                      | type_qualifier {
                          $$ = ast_node_new_list_node($1);
                        }
                      | declaration_specifiers type_qualifier {
                          $$ = ast_node_append($1, $2);
                        }
                      | function_specifier {
                          $$ = ast_node_new_list_node($1);
                        }
                      | declaration_specifiers function_specifier {
                          $$ = ast_node_append($1, $2);
                        }
                      | alignment_specifier {
                          $$ = ast_node_new_list_node($1);
                        }
                      | declaration_specifiers alignment_specifier {
                          $$ = ast_node_append($1, $2);
                        }
                      ;

init_declarator_list: init_declarator {
                        $$ = ast_node_new_list_node($1);
                      }
                    | init_declarator_list ',' init_declarator {
                        $$ = ast_node_append($1, $3);
                      }
                    ;

init_declarator: declarator
               | declarator '=' initializer {
                   $declarator->declaration.assignment = $initializer;
                 }
               ;

storage_class_specifier: TYPEDEF {
                            $$ = ast_node_new_gkcc_storage_class_specifier_node(GKCC_STORAGE_CLASS_SPECIFIER_TYPEDEF);
                         }
                       | EXTERN {
                           $$ = ast_node_new_gkcc_storage_class_specifier_node(GKCC_STORAGE_CLASS_SPECIFIER_EXTERN);
                         }
                       | STATIC {
                           $$ = ast_node_new_gkcc_storage_class_specifier_node(GKCC_STORAGE_CLASS_SPECIFIER_EXTERN);
                         }
                       | _THREAD_LOCAL {
                           $$ = ast_node_new_gkcc_storage_class_specifier_node(GKCC_STORAGE_CLASS_SPECIFIER_THREAD_LOCAL);
                         }
                       | AUTO {
                           $$ = ast_node_new_gkcc_storage_class_specifier_node(GKCC_STORAGE_CLASS_SPECIFIER_AUTO);
                         }
                       | REGISTER {
                           $$ = ast_node_new_gkcc_storage_class_specifier_node(GKCC_STORAGE_CLASS_SPECIFIER_REGISTER);
                         }
                       ;

type_specifier: VOID {
                  $$ = ast_node_new_gkcc_type_specifier_node(GKCC_TYPE_SPECIFIER_VOID);
                }
              | CHAR {
                  $$ = ast_node_new_gkcc_type_specifier_node(GKCC_TYPE_SPECIFIER_CHAR);
                }
              | SHORT {
                  $$ = ast_node_new_gkcc_type_specifier_node(GKCC_TYPE_SPECIFIER_SHORT);
                }
              | INT {
                  $$ = ast_node_new_gkcc_type_specifier_node(GKCC_TYPE_SPECIFIER_INT);
                }
              | LONG {
                  $$ = ast_node_new_gkcc_type_specifier_node(GKCC_TYPE_SPECIFIER_LONG);
                }
              | FLOAT {
                  $$ = ast_node_new_gkcc_type_specifier_node(GKCC_TYPE_SPECIFIER_FLOAT);
                }
              | DOUBLE {
                  $$ = ast_node_new_gkcc_type_specifier_node(GKCC_TYPE_SPECIFIER_DOUBLE);
                }
              | SIGNED {
                  $$ = ast_node_new_gkcc_type_specifier_node(GKCC_TYPE_SPECIFIER_SIGNED);
                }
              | UNSIGNED {
                  $$ = ast_node_new_gkcc_type_specifier_node(GKCC_TYPE_SPECIFIER_UNSIGNED);
                }
              | struct_or_union_specifier {
                  $$ = $1;
                }
              | enum_specifier {
                  $$ = $1;
                }
              //| typedef_name // NOT IMPLEMENTED
              //;

struct_or_union_specifier: struct_or_union identifier {
                             ast_node_update_struct_or_union_specifier_node($struct_or_union, $identifier, NULL, current_symbol_table, YY_FILENAME, yylineno);
                             current_symbol_table = gkcc_symbol_table_set_get_symbol_table_set_of_struct_or_union_node($struct_or_union);
                           } '{' struct_declaration_list '}' {
                             EXIT_SCOPE();
                             $$ = ast_node_update_struct_or_union_specifier_node($struct_or_union, NULL, $struct_declaration_list, current_symbol_table, YY_FILENAME, yylineno);
                           }
                         | struct_or_union {
                             current_symbol_table = gkcc_symbol_table_set_get_symbol_table_set_of_struct_or_union_node($struct_or_union);
                           } '{' struct_declaration_list '}' {
                             EXIT_SCOPE();
                             $$ = ast_node_update_struct_or_union_specifier_node($struct_or_union, NULL, $struct_declaration_list, current_symbol_table, YY_FILENAME, yylineno);
                           }
                         | struct_or_union identifier {
                             $$ = ast_node_update_struct_or_union_specifier_node($struct_or_union, $identifier, NULL, current_symbol_table, YY_FILENAME, yylineno);
                           }
                         ;

struct_or_union: STRUCT {
                   $$ = ast_node_new_struct_or_union_specifier_node(current_symbol_table, GKCC_TYPE_SPECIFIER_STRUCT, NULL);
                 }
               | UNION {
                   $$ = ast_node_new_struct_or_union_specifier_node(current_symbol_table, GKCC_TYPE_SPECIFIER_UNION, NULL);
                 }
               ;

struct_declaration_list: struct_declaration {
                           $$ = ast_node_new_list_node($1);
                         }
                       | struct_declaration_list struct_declaration {
                           $$ = ast_node_append($1, $2);
                         }
                       ;

struct_declaration: specifier_qualifier_list ';' {
                      $$ = ast_node_new_declaration_node($1, NULL);
                    }
                  | specifier_qualifier_list struct_declarator_list ';' {
                      $$ = ast_node_new_declaration_node($1, $2);
                    }
                  // | static_assert_declaration // NOT IMPLEMENTED
                  ;

specifier_qualifier_list: type_specifier {
                            $$ = ast_node_new_list_node($1);
                          }
                        | specifier_qualifier_list type_specifier {
                            $$ = ast_node_append($1, $2);
                          }
                        | type_qualifier {
                            $$ = ast_node_new_list_node($1);
                          }
                        | specifier_qualifier_list type_qualifier {
                            $$ = ast_node_append($1, $2);
                          }
                        ;

struct_declarator_list: struct_declarator {
                          $$ = ast_node_new_list_node($1);
                        }
                      | struct_declarator_list ',' struct_declarator {
                          $$ = ast_node_append($1, $3);
                        }
                      ;

struct_declarator: declarator
                 //| declarator ':' constant_expression // Not implemented
                 //| ':' constant_expression // Not implemented
                 ;

enum_specifier: ENUM '{' enumerator_list '}' {
                  $$ = ast_node_new_enum_definition_node(NULL, $enumerator_list);
                }
              | ENUM identifier '{' enumerator_list '}' {
                  $$ = ast_node_new_enum_definition_node($identifier, $enumerator_list);
                }
              | ENUM '{' enumerator_list ',' '}' {
                  $$ = ast_node_new_enum_definition_node(NULL, $enumerator_list);
                }
              | ENUM identifier '{' enumerator_list ',' '}' {
                  $$ = ast_node_new_enum_definition_node($identifier, $enumerator_list);
                }
              | ENUM identifier {
                  $$ = ast_node_new_enum_definition_node($2, NULL);
                }
              ;

enumerator_list: enumerator {
                   $$ = ast_node_new_list_node($1);
                 }
               | enumerator_list ',' enumerator {
                   $$ = ast_node_append($1, $3);
                 }
               ;

//enumeration_constant: IDENT;

enumerator: identifier
          | identifier '=' constant_expression {
              $$ = ast_node_new_binop_node(AST_BINOP_ASSIGN, $1, $3);
            }
          ;


type_qualifier: CONST {
                  $$ = ast_node_new_gkcc_type_qualifier_node(GKCC_QUALIFIER_CONST);
                }
              | RESTRICT {
                  $$ = ast_node_new_gkcc_type_qualifier_node(GKCC_QUALIFIER_RESTRICT);
                }
              | VOLATILE {
                  $$ = ast_node_new_gkcc_type_qualifier_node(GKCC_QUALIFIER_VOLATILE);
                }
              ;

function_specifier: INLINE {
                      // Not supported
                      $$ = NULL;
                    }
                  | _NORETURN {
                      // Not supported
                      $$ = NULL;
                    }
                  ;

alignment_specifier: _ALIGNAS '(' type_name ')' {
                      // TODO: implement properly
                      $$ = $type_name;
                    }
                  | _ALIGNAS '(' constant_expression ')' {
                      $$ = $constant_expression;
                    }
                  ;

declarator: direct_declarator {
              $$ = ast_node_direct_declarator_to_declarator($direct_declarator);
            }
          | pointer direct_declarator {
              struct ast_node *n = ast_node_append($direct_declarator, $pointer);
              $$ = ast_node_direct_declarator_to_declarator(n);
            }
          ;

direct_declarator: identifier {
                     struct ast_node *n = ast_node_new_declaration_node_from_ident($identifier);
                     $$ = ast_node_new_list_node(n);
                   }
                 | '(' declarator ')' {
                     $$ = ast_node_new_list_node($declarator);
                   }
                 | direct_declarator '[' ']' {
                     struct ast_node *node = ast_node_new_gkcc_array_type_node(NULL);
                     $$ = ast_node_append($1, node);
                   }
                 | direct_declarator '[' assignment_expression ']' {
                     struct ast_node *node = ast_node_new_gkcc_array_type_node($assignment_expression);
                     $$ = ast_node_append($1, node);
                   }
                 | direct_declarator '(' parameter_type_list ')' {
                     struct ast_node *node = ast_node_new_gkcc_function_declarator_with_parameter_type_list($parameter_type_list, NULL);
                     $$ = ast_node_append($1, node);
                   }
                 | direct_declarator '(' identifier_list ')' {
                     // TODO: parse identifier_list properly
                     struct ast_node *node = ast_node_new_gkcc_function_declarator_with_parameter_type_list(NULL, NULL);
                     $$ = ast_node_append($1, node);
                   }
                 | direct_declarator '(' ')' {
                     struct ast_node *node = ast_node_new_gkcc_function_declarator_with_parameter_type_list(NULL, NULL);
                     $$ = ast_node_append($1, node);
                   }
                 ;

pointer: '*' {
           $$ = ast_node_new_list_node(ast_node_new_gkcc_type_node(GKCC_TYPE_PTR));
         }
       | '*' type_qualifier_list {
           // Ignoring these type qualifiers
           // TODO: Maybe include these type qualifiers?
           $$ = ast_node_new_gkcc_type_node(GKCC_TYPE_PTR);
         }
       | '*' type_qualifier_list pointer {
           // Ignoring these type qualifiers
           // TODO: Maybe include these type qualifiers?
           struct ast_node *ptr = ast_node_new_list_node(ast_node_new_gkcc_type_node(GKCC_TYPE_PTR));
           $$ = ast_node_append($3, ptr);
         }
       | '*' pointer {
           struct ast_node *ptr = ast_node_new_list_node(ast_node_new_gkcc_type_node(GKCC_TYPE_PTR));
           $$ = ast_node_append($2, ptr);
         }
       ;

type_qualifier_list: type_qualifier {
                       $$ = ast_node_new_list_node($1);
                     }
                   | type_qualifier_list type_qualifier {
                       $$ = ast_node_append($1, $2);
                     }
                   ;

parameter_type_list: parameter_list {
                       $$ = ast_node_new_list_node($1);
                     }
                   | parameter_list ',' ELLIPSIS {
                       // Not implemented
                       $$ = $parameter_list;
                     }
                   ;

parameter_list: parameter_declaration {
                  $$ = ast_node_new_list_node($1);
                }
              | parameter_list ',' parameter_declaration {
                  $$ = ast_node_append($1, $3);
                }
              ;

parameter_declaration: declaration_specifiers {
                         $$ = ast_node_new_list_node($declaration_specifiers);
                       }
                     | declaration_specifiers declarator {
                         $$ = ast_node_append($declaration_specifiers, $declarator);
                       }
                     //| declaration_specifiers abstract_declarator // NOT IMPLEMENTED
                     ;

identifier_list: identifier {
                   $$ = ast_node_new_list_node($1);
                 }
               | identifier_list ',' identifier {
                   $$ = ast_node_append($1, $3);
                 }
               ;

type_name: specifier_qualifier_list
         // | specifier_qualifier_list abstract_declarator // NOT IMPLEMENTED
         ;

// abstract_declarator not implemented
// direct_abstract_declarator not implemented
// typedef_name not implemented

initializer: assignment_expression {
               $$ = $assignment_expression;
             }
           | '{' initializer_list '}' {
               $$ = $initializer_list;
             }
           | '{' initializer_list ',' '}' {
               $$ = $initializer_list;
              }
           ;

initializer_list: initializer {
                    $$ = ast_node_new_list_node($1);
                  }
                | designation initializer {
                    $$ = ast_node_apply_designator_to_all($1, $2);
                  }
                | initializer_list ',' initializer {
                    $$ = ast_node_append($1, $3);
                  }
                | initializer_list ',' designation initializer {
                    struct ast_node *n = ast_node_apply_designator_to_all($3, $4);
                    $$ = ast_node_append($1, n);
                  }
                ;

designation: designator_list '=' {
               $$ = $1;
             };

designator_list: designator {
                   $$ = ast_node_new_list_node($1);
                 }
               | designator_list designator {
                   $$ = ast_node_append($1, $2);
                 }
               ;

designator: '[' constant_expression ']' {
              $$ = $constant_expression;
            }
          | '.' identifier {
              $$ = $identifier;
            }
          ;

// static_assert_declaration: _STATIC_ASSERT '(' constant_expression ',' STRING ')' ';'; // NOT IMPLEMENTED

// === BEGIN STATEMENTS ===

compound_statement: '{' '}' {
                      $$ = NULL;
                    }
                  | '{' block_item_list '}' {
                      $$ = $2;
                    }
                  ;


statement: labeled_statement {
             $$ = $1;
           }
         | compound_statement {
             $$ = $1;
           }
         | expression_statement {
             $$ = $1;
           }
         | selection_statement {
             $$ = $1;
           }
         | iteration_statement {
             $$ = $1;
           }
         | jump_statement {
             $$ = $1;
           }
         ;

labeled_statement: IDENT ':' statement {
                     struct ast_node *ident_node = yylval2ast_node_ident(&$IDENT);
                     $$ = ast_node_new_goto_node(ident_node);
                     gkcc_scope_add_label_to_scope(current_symbol_table, $$, $statement, YY_FILENAME, yylineno);
                   }
                 | CASE constant_expression ':' statement {
                     $$ = ast_node_new_switch_case_case_node($constant_expression, $statement);
                   }
                 | DEFAULT ':' statement {
                     $$ = ast_node_new_switch_case_case_node(NULL, $statement);
                   }
                 ;

block_item_list: block_item {
                   $$ = ast_node_new_list_node($1);
                 }
               | block_item_list block_item {
                   $$ = ast_node_append($1, $2);
                 }
               ;

block_item: declaration {
              $$ = $1;
            }
          | statement {
              $$ = $1;
            }
          ;

expression_statement: ';' {
                        $$ = NULL;
                      }
                    | expression ';' {
                        $$ = $expression;
                      }
                    ;

selection_statement: IF '(' expression ')' enter_block_symbol_table_set statement %prec IF {
                       $$ = ast_node_new_if_statement($expression, $statement, NULL);
                       EXIT_SCOPE();
                     }
                   | IF '(' expression ')' enter_block_symbol_table_set statement[then_statement] ELSE %prec ELSE {
                       EXIT_SCOPE();
                       ENTER_SCOPE(GKCC_SCOPE_BLOCK);
                     } statement[else_statement] {
                       $$ = ast_node_new_if_statement($expression, $then_statement, $else_statement);
                       EXIT_SCOPE();
                     }
                   | SWITCH '(' expression ')' statement {
                       $$ = ast_node_new_switch_case_switch_node($expression, $statement);
                     }
                   ;

iteration_statement: WHILE '(' expression ')' enter_block_symbol_table_set statement {
                       $$ = ast_node_new_for_loop(NULL, $expression, NULL, $statement);
                     }
                   | DO statement WHILE '(' expression ')' ';' {
                       $$ = ast_node_new_do_while_loop($expression, $statement);
                     }
                   | FOR '(' ';' ';' ')' statement {
                       $$ = ast_node_new_for_loop(NULL, NULL, NULL, $statement);
                     }
                   | FOR '(' expression ';' ';' ')' statement {
                       $$ = ast_node_new_for_loop($expression, NULL, NULL, $statement);
                     }
                   | FOR '(' ';' expression ';' ')' statement {
                       $$ = ast_node_new_for_loop(NULL, $expression, NULL, $statement);
                     }
                   | FOR '(' expression[expr1] ';' expression[expr2] ';' ')' statement {
                       $$ = ast_node_new_for_loop($expr1, $expr2, NULL, $statement);
                     }
                   | FOR '(' ';' ';' expression ')' statement {
                       $$ = ast_node_new_for_loop(NULL, NULL, $expression, $statement);
                     }
                   | FOR '(' expression[expr1] ';' ';' expression[expr3] ')' statement {
                       $$ = ast_node_new_for_loop($expr1, NULL, $expr3, $statement);
                     }
                   | FOR '(' expression[expr1] ';' expression[expr2]';' expression[expr3]')' statement {
                       $$ = ast_node_new_for_loop($expr1, $expr2, $expr3, $statement);
                     }
                   ;

jump_statement: GOTO IDENT ';' {
                  struct ast_node *ident_node = yylval2ast_node_ident(&$IDENT);
                  $$ = ast_node_new_goto_node(ident_node);
                  gkcc_scope_add_label_to_scope(current_symbol_table, $$, NULL, YY_FILENAME, yylineno);
                }
              | CONTINUE ';' {
                  $$ = ast_node_new(AST_NODE_JUMP_CONTINUE);
                }
              | BREAK ';' {
                  $$ = ast_node_new(AST_NODE_JUMP_BREAK);
                }
              | RETURN ';' {
                  $$ = ast_node_new_function_return(NULL);
                }
              | RETURN expression ';' {
                  $$ = ast_node_new_function_return(ast_node_strip_single_list($expression));
                }
              ;

// ==================================
// === BEGIN EXTERNAL DEFINITIONS ===
// ==================================

function_definition: declaration_specifiers declarator {
                       gkcc_scope_add_variable_to_scope(current_symbol_table, ast_node_new_list_node($declarator), yylineno, YY_FILENAME);
                       current_symbol_table = gkcc_symbol_table_set_new(current_symbol_table, GKCC_SCOPE_FUNCTION);
                     } compound_statement {
                       $$ = ast_node_new_function_definition_node($declaration_specifiers, $declarator, NULL, $compound_statement);
                       EXIT_SCOPE();
                     }
                   //| declaration_specifiers declarator declaration_list compound_statement
                   // NO, I am not supporting this old syntax (actually, on second thought, maybe later)
                   // TODO: Should this be done?
                   ;

declaration_list: declaration {
                    $$ = ast_node_new_list_node($1);
                  }
                | declaration_list declaration {
                    $$ = ast_node_append($1, $2);
                  }
                ;

enter_block_symbol_table_set: %empty {
                                ENTER_SCOPE(GKCC_SCOPE_BLOCK);
                              }


// Copyright (C) 2023 Gary Kim <gary@garykim.dev>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef AST_H
#define AST_H

#include "lex_extras.h"

enum ast_binop_type {
  AST_BINOP_ADD = 1,
  AST_BINOP_SUBTRACT,
  AST_BINOP_DIVIDE,
  AST_BINOP_MULTIPLY,
  AST_BINOP_MOD,
  AST_BINOP_ASSIGN,
};

struct ast_binop {
  enum ast_binop_type type;
  struct ast_node *left;
  struct ast_node *right;
};

struct ast_ident {
  char *name;
};

enum ast_constant_type {
  AST_CONSTANT_LONGLONG = 1,
  AST_CONSTANT_LONG_DOUBLE,
  AST_CONSTANT_DOUBLE,
  AST_CONSTANT_FLOAT,
  AST_CONSTANT_LONG,
  AST_CONSTANT_INT,
  AST_CONSTANT_CHAR,
};
struct ast_constant {
  enum ast_constant_type type;
  union {
    long long ylonglong;
    long double ylongdouble;
    double ydouble;
    float yfloat;
    long ylong;
    int yint;
    char ychar;
  };
};

enum ast_node_type {
  AST_NODE_BINOP = 1,
  AST_NODE_CONSTANT,
  AST_NODE_IDENT,
};
struct ast_node {
  enum ast_node_type type;
  union {
    struct ast_binop binop;
    struct ast_constant constant;
  };
};

struct ast_node* ast_node_new(enum ast_node_type node_type);
void ast_node_string(char* buf, struct ast_node* node);
struct ast_node* yylval2ast_node(struct _yylval* yylval);
void yynum2ast_node(struct ast_node* node, struct _yynum* yynum);
#endif

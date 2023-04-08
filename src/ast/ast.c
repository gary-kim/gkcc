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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lex_extras.h"
#include "ast.h"

struct ast_node* ast_node_new(enum ast_node_type node_type) {
  struct ast_node* ast_node = calloc(1, sizeof(struct ast_node));
  ast_node->type = node_type;
  return ast_node;
}

void ast_node_string(char* buf, struct ast_node* node) {
  switch (node->type) {
  case AST_NODE_BINOP:
    sprintf(buf, "BINARY OP %d", node->type);
    break;
  case AST_NODE_CONSTANT:
    sprintf(buf, "CONSTANT: (type=%d) %d", node->constant.type, node->constant.yint);
    break;
  case AST_NODE_IDENT:
    break;
  }
}

struct ast_node* yylval2ast_node(struct _yylval* yylval) {
  struct ast_node *node = ast_node_new(0);
  switch (yylval->type) {
  case YYLVAL_TYPE_NUMBER:
    yynum2ast_node(node, &yylval->data.number);
    break;
  case YYLVAL_TYPE_CHAR:
    node->type = AST_NODE_CONSTANT;
    node->constant.type = AST_CONSTANT_CHAR;
    node->constant.ychar = yylval->data.character;
  }
}

void yynum2ast_node(struct ast_node* node, struct _yynum* yynum) {
  node->type = AST_NODE_CONSTANT;
  switch (yynum->type) {
  case YYNUM_TYPE_LONGLONG:
    node->constant.type = AST_CONSTANT_LONGLONG;
    node->constant.ylonglong = yynum->num.ylonglong;
    break;
  case YYNUM_TYPE_LONG_DOUBLE:
    node->constant.type = AST_CONSTANT_LONG_DOUBLE;
    node->constant.ylongdouble = yynum->num.ylongdouble;
    break;
  case YYNUM_TYPE_DOUBLE:
    node->constant.type = AST_CONSTANT_DOUBLE;
    node->constant.ydouble = yynum->num.ydouble;
    break;
  case YYNUM_TYPE_FLOAT:
    node->constant.type = AST_CONSTANT_FLOAT;
    node->constant.yfloat = yynum->num.yfloat;
    break;
  case YYNUM_TYPE_LONG:
    node->constant.type = AST_CONSTANT_LONG;
    node->constant.ylong = yynum->num.ylong;
    break;
  case YYNUM_TYPE_INT:
    node->constant.type = AST_CONSTANT_INT;
    node->constant.yint = yynum->num.yint;
    break;
  }
}

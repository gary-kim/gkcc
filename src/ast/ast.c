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

#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lex_extras.h"

char flbuf[1024];

void ast_print(struct ast_node *top, int depth) {
  char buf[1024];
  if (top == NULL) {
    return;
  }

  int start_of_line = 2 * depth;

  for (int i = 0; i < start_of_line; ++i) {
    buf[i] = ' ';
  }

  ast_node_string(&buf[start_of_line], top);

  printf("%s\n", buf);

  switch (top->type) {
    case AST_NODE_BINOP:
      ast_print(top->binop.left, depth + 1);
      ast_print(top->binop.right, depth + 1);
      break;
    case AST_NODE_CONSTANT:
      break;
    case AST_NODE_IDENT:
      break;
    case AST_NODE_UNARY:
      ast_print(top->unary.of, depth + 1);
      break;
  }
}

struct ast_node *ast_node_new(enum ast_node_type node_type) {
  struct ast_node *ast_node = calloc(1, sizeof(struct ast_node));
  ast_node->type = node_type;
  return ast_node;
}

void ast_node_string(char *buf, struct ast_node *node) {
  switch (node->type) {
    case AST_NODE_BINOP:
      sprintf(buf, "BINARY OP %s", ast_binop_type_string(&node->binop));
      break;
    case AST_NODE_CONSTANT:
      strcpy(buf, ast_constant_string(&node->constant));
      break;
    case AST_NODE_IDENT:
      break;
    case AST_NODE_UNARY:
      strcpy(buf, ast_unary_string(&node->unary));
      break;
  }
}

const char *ast_binop_type_string(struct ast_binop *binop) {
  return AST_BINOP_TYPE_STRING[binop->type];
}

char *ast_constant_string(struct ast_constant* constant) {
  switch (constant->type) {
    case AST_CONSTANT_LONGLONG:
        sprintf(flbuf, "CONSTANT: (type=LONG LONG) %lld", constant->ylonglong);
      break;
    case AST_CONSTANT_LONG_DOUBLE:
      sprintf(flbuf, "CONSTANT: (type=LONG DOUBLE) %Lf", constant->ylongdouble);
      break;
    case AST_CONSTANT_DOUBLE:
      sprintf(flbuf, "CONSTANT: (type=DOUBLE) %f", constant->ydouble);
      break;
    case AST_CONSTANT_FLOAT:
      sprintf(flbuf, "CONSTANT: (type=FLOAT) %f", constant->yfloat);
      break;
    case AST_CONSTANT_LONG:
      sprintf(flbuf, "CONSTANT: (type=LONG) %ld", constant->ylong);
      break;
    case AST_CONSTANT_INT:
      sprintf(flbuf, "CONSTANT: (type=INT) %d", constant->yint);
      break;
    case AST_CONSTANT_CHAR:
      sprintf(flbuf, "CONSTANT: (type=CHAR) %c", constant->ychar);
      break;
  }
  return flbuf;
}

char *ast_unary_string(struct ast_unary *node) {
  sprintf(flbuf, "UNARY OP %s", AST_UNARY_TYPE_STRING[node->type]);
  return flbuf;
}

struct ast_node *yylval2ast_node(struct _yylval *yylval) {
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

void yynum2ast_node(struct ast_node *node, struct _yynum *yynum) {
  node->type = AST_NODE_CONSTANT;
  node->constant.is_unsigned = yynum->is_unsigned;
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

struct ast_node *ast_node_new_binop_node(enum ast_binop_type type,
                                         struct ast_node *left,
                                         struct ast_node *right) {
  struct ast_node *node = ast_node_new(AST_NODE_BINOP);
  node->binop.type = type;
  node->binop.left = left;
  node->binop.right = right;
  return node;
}

struct ast_node *ast_node_new_unary_node(enum ast_unary_type type,
                                         struct ast_node *of) {
  struct ast_node *node = ast_node_new(AST_NODE_UNARY);
  node->unary.of = of;
  node->unary.type = type;
  return node;
}

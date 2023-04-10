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
#include "misc.h"

char flbuf[1024];

void ast_print(struct ast_node *top, int depth, char *prefix) {
  char buf[1024];
  if (top == NULL) {
    return;
  }

  // Don't want the extra empty newline of AST_NODE_LIST
  if (prefix != NULL) {
    int start_of_line = 2 * depth;

    for (int i = 0; i < start_of_line; ++i) {
      buf[i] = ' ';
    }

    for (int i = 0; prefix[i] != '\0'; i++) {
      buf[start_of_line++] = prefix[i];
    }

    ast_node_string(&buf[start_of_line], top);

    printf("%s\n", buf);
  }

  switch (top->type) {
    case AST_NODE_BINOP:
      ast_print(top->binop.left, depth + 1, "expr 1: ");
      ast_print(top->binop.right, depth + 1, "expr 2: ");
      break;
    case AST_NODE_CONSTANT:
      break;
    case AST_NODE_IDENT:
      break;
    case AST_NODE_UNARY:
      ast_print(top->unary.of, depth + 1, "");
      break;
    case AST_NODE_TERNARY:
      ast_print(top->ternary.condition, depth + 1, "condition: ");
      ast_print(top->ternary.true_expr, depth + 1, "true_expr: ");
      ast_print(top->ternary.false_expr, depth + 1, "false_expr: ");
      break;
    case AST_NODE_GKCC_TYPE:
      // TODO: This should be removed once declarations are done
      ast_print(top->gkcc_type.child, depth + 1, "");
      break;
    case AST_NODE_DECLARATION:
      ast_print(top->declaration.specifiers, depth + 1, "specifiers: ");
      ast_print(top->declaration.init_declarator, depth + 1,
                "init_declarator: ");
      ast_print(top->declaration.assignment, depth + 1, "assignment: ");
      break;
    case AST_NODE_LIST:
      ast_print(top->list.node, depth + 1, "list_node: ");
      ast_print(top->list.next, depth, NULL);
      break;
    case AST_NODE_TOP_LEVEL:
      ast_print(top->top_level.list, depth + 1, NULL);
      break;
  }
}

struct ast_node *ast_node_new(enum ast_node_type node_type) {
  struct ast_node *ast_node = malloc(sizeof(struct ast_node));
  memset(ast_node, 0, sizeof(struct ast_node));
  ast_node->type = node_type;
  return ast_node;
}

void ast_node_string(char *buf, struct ast_node *node) {
  char extrabuf[8193];
  switch (node->type) {
    case AST_NODE_BINOP:
      sprintf(buf, "%s: %s", AST_NODE_TYPE_STRING[AST_NODE_BINOP],
              ast_binop_type_string(&node->binop));
      break;
    case AST_NODE_CONSTANT:
      strcpy(buf, ast_constant_string(&node->constant));
      break;
    case AST_NODE_IDENT:
      sprint_escaped_string(extrabuf, node->ident.name, node->ident.length);
      sprintf(buf, "%s: %s", AST_NODE_TYPE_STRING[AST_NODE_IDENT], extrabuf);
      break;
    case AST_NODE_UNARY:
      strcpy(buf, ast_unary_string(&node->unary));
      break;
    case AST_NODE_TERNARY:
      strcpy(buf, ast_ternary_string(&node->ternary));
      break;
    case AST_NODE_GKCC_TYPE:
      strcpy(buf, ast_gkcc_type_string(&node->gkcc_type));
      break;
    case AST_NODE_LIST:
      buf[0] = '\0';
      break;
    default:
      sprintf(buf, "%s:", AST_NODE_TYPE_STRING[node->type]);
      break;
  }
}

const char *ast_binop_type_string(struct ast_binop *binop) {
  return AST_BINOP_TYPE_STRING[binop->type];
}

char *ast_gkcc_type_string(struct ast_gkcc_type *gkcc_type) {
  const char *type_type = GKCC_TYPE_TYPE_STRING[gkcc_type->gkcc_type->type];
  switch (gkcc_type->gkcc_type->type) {
    case GKCC_TYPE_SCALAR:
      break;
    case GKCC_TYPE_FUNCTION:
      break;
    case GKCC_TYPE_PTR:
      break;
    case GKCC_TYPE_ARRAY:
      break;
    case GKCC_TYPE_STRUCT:
      break;
    case GKCC_TYPE_UNION:
      break;
    case GKCC_TYPE_QUALIFIER:
    case GKCC_TYPE_STORAGE_CLASS_SPECIFIER:
    case GKCC_TYPE_TYPE_SPECIFIER:
      sprintf(flbuf, "GKCC_TYPE (type=%s): %s", type_type,
              GKCC_TYPE_SPECIFIER_TYPE_STRING[gkcc_type->gkcc_type
                                                  ->type_specifier.type]);
      break;
  }
  return flbuf;
}

char *ast_constant_string(struct ast_constant *constant) {
  char local_buf[10000];
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
      sprint_escaped_char(local_buf, constant->ychar);
      sprintf(flbuf, "CONSTANT: (type=CHAR) '%s'", local_buf);
      break;
    case AST_CONSTANT_STRING:
      sprint_escaped_string(local_buf, constant->ystring.raw, constant->ystring.length);
      sprintf(flbuf, "CONSTANT: (type=STRING) \"%s\"", local_buf);
      break;
  }
  return flbuf;
}

char *ast_unary_string(struct ast_unary *node) {
  sprintf(flbuf, "UNARY OP %s", AST_UNARY_TYPE_STRING[node->type]);
  return flbuf;
}

char *ast_ternary_string(struct ast_ternary *node) {
  sprintf(flbuf, "TERNARY:");
  return flbuf;
}

struct ast_node *ast_node_new_constant_int_node(int val) {
  struct ast_node *node = ast_node_new(AST_NODE_CONSTANT);
  node->constant.type = AST_CONSTANT_INT;
  node->constant.yint = val;
  return node;
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

struct ast_node *ast_node_append(struct ast_node *parent,
                                 struct ast_node *child) {
  gkcc_assert(child != NULL, GKCC_ERROR_INVALID_ARGUMENTS,
              "");

  // if parent == NULL, the user probably wanted to create a new list node
  // from scratch. Do it for them.
  if (parent == NULL) {
    return ast_node_new_list_node(child);
  }

  gkcc_assert(parent->type == AST_NODE_LIST, GKCC_ERROR_INVALID_ARGUMENTS,
              "ast_node_append given an ast_node that is not AST_NODE_LIST");
  struct ast_node *last_node = parent->list.end;
  struct ast_node *list_child = NULL;

  if(child->type == AST_NODE_LIST) {
    // Flatten the list
    for(struct ast_node *n = child; n != NULL; n = n->list.next) {
      list_child = ast_node_new_list_node(n->list.node);
      last_node->list.next = list_child;
      last_node = list_child;
    }
  } else {
    list_child = ast_node_new_list_node(child);
    last_node->list.next = list_child;
  }
  parent->list.end = list_child;
  return parent;
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

struct ast_node *ast_node_new_ternary_node(struct ast_node *condition,
                                           struct ast_node *true_expr,
                                           struct ast_node *false_expr) {
  struct ast_node *node = ast_node_new(AST_NODE_TERNARY);
  node->ternary.condition = condition;
  node->ternary.true_expr = true_expr;
  node->ternary.false_expr = false_expr;
  return node;
}

struct ast_node *ast_node_new_gkcc_type_qualifier_node(
    enum gkcc_qualifier_type qualifier_type, struct ast_node *child) {
  struct ast_node *node = ast_node_new(AST_NODE_GKCC_TYPE);
  node->gkcc_type.child = child;
  node->gkcc_type.gkcc_type = gkcc_type_new(GKCC_TYPE_QUALIFIER);
  node->gkcc_type.gkcc_type->qualifier.type = qualifier_type;
  return node;
}

struct ast_node *ast_node_new_gkcc_storage_class_specifier_node(
    enum gkcc_storage_class_specifier_type type, struct ast_node *child) {
  struct ast_node *node = ast_node_new(AST_NODE_GKCC_TYPE);
  node->gkcc_type.child = child;
  node->gkcc_type.gkcc_type = gkcc_type_new(GKCC_TYPE_TYPE_SPECIFIER);
  node->gkcc_type.gkcc_type->type = GKCC_TYPE_STORAGE_CLASS_SPECIFIER;
  node->gkcc_type.gkcc_type->storage_class_specifier.type = type;
  return node;
}

struct ast_node *ast_node_new_gkcc_type_specifier_node(
    enum gkcc_type_specifier_type type, struct ast_node *child) {
  struct ast_node *node = ast_node_new(AST_NODE_GKCC_TYPE);
  node->gkcc_type.child = child;
  node->gkcc_type.gkcc_type = gkcc_type_new(GKCC_TYPE_TYPE_SPECIFIER);
  node->gkcc_type.gkcc_type->type = GKCC_TYPE_TYPE_SPECIFIER;
  node->gkcc_type.gkcc_type->type_specifier.type = type;
  return node;
}

struct ast_node *ast_node_new_declaration_node(
    struct ast_node *declaration_specifiers,
    struct ast_node *init_declarator_list) {
  gkcc_assert(init_declarator_list->type == AST_NODE_LIST,
              GKCC_ERROR_INVALID_ARGUMENTS,
              "ast_node_new_declaration_node() got an init_declarator_list "
              "that is not a list type");
  struct ast_node *list_node = NULL;
  for (struct ast_node *n = init_declarator_list; n != NULL; n = n->list.next) {
    struct ast_node *node = ast_node_new(AST_NODE_DECLARATION);
    struct ast_node *referenced_node = n->list.node;

    node->declaration.specifiers = declaration_specifiers;
    if (referenced_node->type == AST_NODE_BINOP && referenced_node->binop.type == AST_BINOP_ASSIGN) {
      // The init-declarator has an assigned value
      node->declaration.init_declarator = referenced_node->binop.left;
      node->declaration.assignment = referenced_node->binop.right;
    } else {
      // the init-declarator does not have an assigned value;
      node->declaration.init_declarator = referenced_node;
      node->declaration.assignment = NULL;
    }
    list_node = ast_node_append(list_node, node);
  }
  return list_node;
}

struct ast_node *ast_node_new_list_node(struct ast_node *node) {
  if (node->type == AST_NODE_LIST) // We should never need a list of list
    return node;

  struct ast_node *new_node = ast_node_new(AST_NODE_LIST);
  new_node->list.node = node;
  new_node->list.next = NULL;
  new_node->list.end = new_node;
  return new_node;
}

struct ast_node *yylval2ast_node_ident(struct _yylval *yylval) {
  struct ast_node *node = ast_node_new(AST_NODE_IDENT);
  gkcc_assert(yylval->type == YYLVAL_TYPE_STRING, GKCC_ERROR_INVALID_ARGUMENTS,
              "yylval2ast_node_ident() was given a non-string yylval");

  node->ident.length = yylval->data.string.length;
  node->ident.name = malloc(node->constant.ystring.length);
  memcpy(node->ident.name, yylval->data.string.string, node->ident.length);

  return node;
}

struct ast_node *yylval2ast_node(struct _yylval *yylval) {
  struct ast_node *node = ast_node_new(AST_NODE_UNKNOWN);
  switch (yylval->type) {
    case YYLVAL_TYPE_NUMBER:
      yynum2ast_node(node, &yylval->data.number);
      break;
    case YYLVAL_TYPE_CHAR:
      node->type = AST_NODE_CONSTANT;
      node->constant.type = AST_CONSTANT_CHAR;
      node->constant.ychar = yylval->data.character;
      break;
    case YYLVAL_TYPE_STRING:
      node->type = AST_NODE_CONSTANT;
      node->constant.type = AST_CONSTANT_STRING;

      node->constant.ystring.length = yylval->data.string.length;
      node->constant.ystring.raw = malloc(node->constant.ystring.length);
      memcpy(node->constant.ystring.raw, yylval->data.string.string,
             node->constant.ystring.length);
      break;
  }
  return node;
}

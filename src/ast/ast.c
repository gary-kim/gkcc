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

void ast_print(struct ast_node *top, int depth, const char *prefix) {
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
      ast_print(top->top_level.list, depth, NULL);
      break;
    case AST_NODE_FUNCTION_CALL:
      ast_print(top->function_call.name, depth + 1, "function_name: ");
      ast_print(top->function_call.parameters, depth + 1, "parameters: ");
      break;
    case AST_NODE_FUNCTION_DEFINITION:
      ast_print(top->function_definition.returns, depth + 1, "returns: ");
      ast_print(top->function_definition.name, depth + 1, "name: ");
      ast_print(top->function_definition.parameters, depth + 1, "parameters: ");
      ast_print(top->function_definition.statements, depth + 1, "statements: ");
      break;
    case AST_NODE_ENUM_DEFINITION:
      ast_print(top->enum_definition.enumerators, depth + 1, "enumerators: ");
      ast_print(top->enum_definition.ident, depth + 1, "ident: ");
      break;
    case AST_NODE_STRUCT_OR_UNION_DEFINITION:
      ast_print(top->struct_or_union_definition.ident, depth + 1, "ident: ");
      ast_print(top->struct_or_union_definition.members, depth + 1,
                "members: ");
      break;
    case AST_NODE_IF_STATEMENT:
      ast_print(top->if_statement.condition, depth + 1, "condition: ");
      ast_print(top->if_statement.then_statement, depth + 1, "then: ");
      ast_print(top->if_statement.else_statement, depth + 1, "else: ");
      break;
    case AST_NODE_FOR_LOOP:
      ast_print(top->for_loop.expr1, depth + 1, "expr1: ");
      ast_print(top->for_loop.expr2, depth + 1, "expr2: ");
      ast_print(top->for_loop.expr3, depth + 1, "expr3: ");
      ast_print(top->for_loop.statements, depth + 1, "do: ");
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
    case AST_NODE_STRUCT_OR_UNION_DEFINITION:
      sprintf(buf, "%s (type=%s): ", AST_NODE_TYPE_STRING[node->type],
              AST_STRUCT_OR_UNION_DEFINITION_TYPE_STRING
                  [node->struct_or_union_definition.type]);
      break;
    case AST_NODE_FOR_LOOP:
      if (node->for_loop.is_do_while)
        strcpy(extrabuf, "true");
      else
        strcpy(extrabuf, "false");
      sprintf(buf, "%s (is_do_while=%s):", AST_NODE_TYPE_STRING[node->type],
              extrabuf);
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
      sprint_escaped_string(local_buf, constant->ystring.raw,
                            constant->ystring.length);
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
  // if parent == NULL, the user probably wanted to create a new list node
  // from scratch. Do it for them.
  if (parent == NULL) {
    return ast_node_new_list_node(child);
  }

  // Sometimes, the child is NULL because there is an attempt to append a
  // expression or other ast_node that actually turned out to be empty.
  // In that case, just return the parent directly.
  if (child == NULL) return parent;

  gkcc_assert(parent->type == AST_NODE_LIST, GKCC_ERROR_INVALID_ARGUMENTS,
              "ast_node_append given an ast_node that is not AST_NODE_LIST");
  struct ast_node *last_node = parent->list.end;
  struct ast_node *list_child = NULL;

  if (child->type == AST_NODE_LIST) {
    // Flatten the list
    for (struct ast_node *n = child; n != NULL; n = n->list.next) {
      // Sometimes, list nodes of nothing are made for compatability reasons
      // Throw it away at this stage.
      if (n->list.node == NULL) continue;
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

struct ast_node *ast_node_new_if_statement(struct ast_node *condition,
                                           struct ast_node *then_statement,
                                           struct ast_node *else_statement) {
  struct ast_node *node = ast_node_new(AST_NODE_IF_STATEMENT);
  node->if_statement.condition = condition;
  node->if_statement.then_statement = then_statement;
  node->if_statement.else_statement = else_statement;
  return node;
}
struct ast_node *ast_node_new_for_loop(struct ast_node *expr1,
                                       struct ast_node *expr2,
                                       struct ast_node *expr3,
                                       struct ast_node *statements) {
  struct ast_node *node = ast_node_new(AST_NODE_FOR_LOOP);
  node->for_loop.is_do_while = false;
  node->for_loop.expr1 = expr1;
  node->for_loop.expr2 = expr2;
  node->for_loop.expr3 = expr3;
  node->for_loop.statements = statements;
  return node;
}

struct ast_node *ast_node_new_do_while_loop(struct ast_node *condition,
                                            struct ast_node *statements) {
  struct ast_node *node = ast_node_new(AST_NODE_FOR_LOOP);
  node->for_loop.is_do_while = true;
  node->for_loop.expr1 = NULL;
  node->for_loop.expr2 = condition;
  node->for_loop.expr3 = NULL;
  node->for_loop.statements = statements;
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

struct ast_node *ast_node_new_gkcc_type_node(enum gkcc_type_type type) {
  struct ast_node *node = ast_node_new(AST_NODE_GKCC_TYPE);
  node->gkcc_type.gkcc_type = gkcc_type_new(type);
  return node;
}

struct ast_node *ast_node_new_gkcc_type_qualifier_node(
    enum gkcc_qualifier_type qualifier_type) {
  struct ast_node *node = ast_node_new_gkcc_type_node(GKCC_TYPE_QUALIFIER);
  node->gkcc_type.gkcc_type->qualifier.type = qualifier_type;
  return node;
}

struct ast_node *ast_node_new_gkcc_storage_class_specifier_node(
    enum gkcc_storage_class_specifier_type type) {
  struct ast_node *node = ast_node_new(AST_NODE_GKCC_TYPE);
  node->gkcc_type.gkcc_type = gkcc_type_new(GKCC_TYPE_TYPE_SPECIFIER);
  node->gkcc_type.gkcc_type->type = GKCC_TYPE_STORAGE_CLASS_SPECIFIER;
  node->gkcc_type.gkcc_type->storage_class_specifier.type = type;
  return node;
}

struct ast_node *ast_node_new_gkcc_type_specifier_node(
    enum gkcc_type_specifier_type type) {
  struct ast_node *node = ast_node_new(AST_NODE_GKCC_TYPE);
  node->gkcc_type.gkcc_type = gkcc_type_new(GKCC_TYPE_TYPE_SPECIFIER);
  node->gkcc_type.gkcc_type->type = GKCC_TYPE_TYPE_SPECIFIER;
  node->gkcc_type.gkcc_type->type_specifier.type = type;
  return node;
}

struct ast_node *ast_node_new_function_definition_node(
    struct ast_node *returns, struct ast_node *function_name,
    struct ast_node *parameters, struct ast_node *statements) {
  struct ast_node *node = ast_node_new(AST_NODE_FUNCTION_DEFINITION);
  node->function_definition.returns = returns;
  node->function_definition.name = function_name;
  node->function_definition.parameters = parameters;
  node->function_definition.statements = statements;
  return node;
}

struct ast_node *ast_node_new_declaration_node(
    struct ast_node *declaration_specifiers,
    struct ast_node *init_declarator_list) {
  gkcc_assert(init_declarator_list == NULL ||
                  init_declarator_list->type == AST_NODE_LIST,
              GKCC_ERROR_INVALID_ARGUMENTS,
              "ast_node_new_declaration_node() got an init_declarator_list "
              "that is not a list type");

  // This is some kind of enum or struct definition
  if (init_declarator_list == NULL) {
    struct ast_node *node = ast_node_new(AST_NODE_DECLARATION);
    node->declaration.specifiers = declaration_specifiers;
    return node;
  }

  struct ast_node *list_node = NULL;
  for (struct ast_node *n = init_declarator_list; n != NULL; n = n->list.next) {
    struct ast_node *node = ast_node_new(AST_NODE_DECLARATION);
    struct ast_node *referenced_node = n->list.node;

    node->declaration.specifiers = declaration_specifiers;
    if (referenced_node->type == AST_NODE_BINOP &&
        referenced_node->binop.type == AST_BINOP_ASSIGN) {
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
  if (node->type == AST_NODE_LIST)  // We should never need a list of list
    return node;

  struct ast_node *new_node = ast_node_new(AST_NODE_LIST);
  new_node->list.node = node;
  new_node->list.next = NULL;
  new_node->list.end = new_node;
  return new_node;
}

struct ast_node *ast_node_new_function_call_node(struct ast_node *function_name,
                                                 struct ast_node *parameters) {
  struct ast_node *new_node = ast_node_new(AST_NODE_FUNCTION_CALL);
  new_node->function_call.name = function_name;
  new_node->function_call.parameters = parameters;
  return new_node;
}

struct ast_node *ast_node_apply_designator_to_all(
    struct ast_node *designators, struct ast_node *initializer) {
  struct ast_node *tr_node = NULL;
  for (struct ast_node *n = designators; n != NULL; n = n->list.next) {
    struct ast_node *to_add =
        ast_node_new_binop_node(AST_BINOP_ASSIGN, n->list.node, initializer);
    tr_node = ast_node_append(tr_node, to_add);
  }
  return tr_node;
}

struct ast_node *ast_node_new_enum_definition_node(
    struct ast_node *ident, struct ast_node *enumerators) {
  struct ast_node *enum_node = ast_node_new(AST_NODE_ENUM_DEFINITION);
  enum_node->enum_definition.ident = ident;
  enum_node->enum_definition.enumerators = enumerators;
  return enum_node;
}

struct ast_node *ast_node_new_struct_or_union_definition_node(
    enum ast_struct_or_union_definition_type type, struct ast_node *ident,
    struct ast_node *members) {
  struct ast_node *node = ast_node_new(AST_NODE_STRUCT_OR_UNION_DEFINITION);
  node->struct_or_union_definition.type = type;
  node->struct_or_union_definition.ident = ident;
  node->struct_or_union_definition.members = members;
  return node;
}

struct ast_node *ast_node_update_struct_or_union_definition_node(
    struct ast_node *node, struct ast_node *ident, struct ast_node *members) {
  gkcc_assert(node->type == AST_NODE_STRUCT_OR_UNION_DEFINITION,
              GKCC_ERROR_INVALID_ARGUMENTS,
              "ast_node_update_struct_or_union_definition_node() got node that "
              "is not AST_NODE_STRUCT_OR_UNION_DEFINITION");

  node->struct_or_union_definition.ident = ident;
  node->struct_or_union_definition.members = members;
  return node;
}

struct ast_node *ast_node_declaration_to_type(struct ast_node *original_node) {
  if (original_node->type != AST_NODE_LIST) {
    original_node = ast_node_new_list_node(original_node);
  }

  struct ast_node *new_node = ast_node_new(AST_NODE_GKCC_TYPE);

  for (struct ast_node *n_list = original_node; n_list != NULL;
       n_list = n_list->list.next) {
    struct ast_node *n = n_list->list.node;
  }
  // TODO: CONTINUE HERE
}

struct ast_node *yylval2ast_node_ident(struct _yylval *yylval) {
  struct ast_node *node = ast_node_new(AST_NODE_IDENT);
  gkcc_assert(yylval->type == YYLVAL_TYPE_STRING, GKCC_ERROR_INVALID_ARGUMENTS,
              "yylval2ast_node_ident() was given a non-string yylval");

  node->ident.length = yylval->data.string.length;
  node->ident.name = malloc(node->ident.length);
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

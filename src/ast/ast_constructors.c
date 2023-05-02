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

#include "ast_constructors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "lex_extras.h"
#include "misc.h"

struct ast_node *ast_node_new_if_statement(struct ast_node *condition,
                                           struct ast_node *then_statement,
                                           struct ast_node *else_statement) {
  struct ast_node *node = ast_node_new(AST_NODE_IF_STATEMENT);
  node->if_statement.condition = condition;
  node->if_statement.then_statement = then_statement;
  node->if_statement.else_statement = else_statement;
  return node;
}

struct ast_node *ast_node_new(enum ast_node_type node_type) {
  struct ast_node *ast_node = malloc(sizeof(struct ast_node));
  memset(ast_node, 0, sizeof(struct ast_node));
  ast_node->type = node_type;
  return ast_node;
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

struct ast_node *ast_node_new_gkcc_array_type_node(struct ast_node *size) {
  struct ast_node *node = ast_node_new_gkcc_type_node(GKCC_TYPE_ARRAY);
  node->gkcc_type.gkcc_type->array.size = size;
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

struct ast_node *ast_node_new_declaration_node_from_ident(
    struct ast_node *ident) {
  struct ast_node *node = ast_node_new(AST_NODE_DECLARATION);
  node->declaration.identifier = ident;
  node->declaration.type = ast_node_new(AST_NODE_GKCC_TYPE);
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

  // TODO: The below comment is not true. Needs to be fixed
  // This is some kind of enum or struct definition
  if (init_declarator_list == NULL) {
    struct ast_node *node = ast_node_new(AST_NODE_DECLARATION);
    node->declaration.type = declaration_specifiers;
    return node;
  }

  struct ast_node *list_node = NULL;
  for (struct ast_node *n = init_declarator_list; n != NULL; n = n->list.next) {
    struct ast_node *referenced_node = n->list.node;

    gkcc_assert(referenced_node->type == AST_NODE_DECLARATION,
                GKCC_ERROR_UNEXPECTED_VALUE,
                "ast_node_new_declaration_node got a init_declarator that is "
                "not a AST_NODE_DECLARATION type");

    struct gkcc_type *gkcc_type =
        ast_node_declaration_specifiers_to_gkcc_data_type(
            declaration_specifiers);
    referenced_node->declaration.type->gkcc_type.gkcc_type = gkcc_type_append(
        referenced_node->declaration.type->gkcc_type.gkcc_type, gkcc_type);

    list_node = ast_node_append(list_node, referenced_node);
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

struct ast_node *ast_node_new_gkcc_function_declarator_with_parameter_type_list(
    struct ast_node *parameter_type_list) {
  // TODO: CONTINUE HERE
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

struct ast_node *ast_node_new_struct_or_union_specifier_node(
    enum ast_struct_or_union_specifier_type type, struct ast_node *ident,
    struct ast_node *members) {
  struct ast_node *node = ast_node_new(AST_NODE_GKCC_TYPE);
  node->gkcc_type.gkcc_type = gkcc_type_new(GKCC_TYPE_TYPE_SPECIFIER);
  node->gkcc_type.gkcc_type->type_specifier.type = type;
  node->gkcc_type.gkcc_type->type_specifier.ident = ident;
  // TODO: Put members in symbol table
  return node;
}

struct ast_node *ast_node_update_struct_or_union_specifier_node(
    struct ast_node *node, struct ast_node *ident, struct ast_node *members) {
  gkcc_assert(node->type == AST_NODE_GKCC_TYPE &&
                  node->gkcc_type.gkcc_type->type == GKCC_TYPE_TYPE_SPECIFIER &&
                  (node->gkcc_type.gkcc_type->type_specifier.type ==
                       GKCC_TYPE_SPECIFIER_STRUCT ||
                   node->gkcc_type.gkcc_type->type_specifier.type ==
                       GKCC_TYPE_SPECIFIER_UNION),
              GKCC_ERROR_INVALID_ARGUMENTS,
              "ast_node_update_struct_or_union_specifier_node() got node that "
              "is not AST_NODE_STRUCT_OR_UNION_SPECIFIER");

  node->gkcc_type.gkcc_type->type_specifier.ident = ident;
  // TODO: Put members in scope table
  return node;
}

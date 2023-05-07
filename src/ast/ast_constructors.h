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

#ifndef GKCC_AST_CONSTRUCTORS_H
#define GKCC_AST_CONSTRUCTORS_H

#include <stdbool.h>

#include "lex_extras.h"
#include "misc.h"
#include "types.h"

// =============================
// === Function Declarations ===
// =============================

struct ast_node *ast_node_new_if_statement(struct ast_node *condition,
                                           struct ast_node *then_statement,
                                           struct ast_node *else_statement);

struct ast_node *ast_node_new_list_node(struct ast_node *node);

struct ast_node *ast_node_new_function_definition_node(
    struct ast_node *returns, struct ast_node *function_name,
    struct ast_node *parameters, struct ast_node *statements);

struct ast_node *ast_node_new_gkcc_storage_class_specifier_node(
    enum gkcc_storage_class_specifier_type type);

struct ast_node *ast_node_new_gkcc_type_qualifier_node(
    enum gkcc_qualifier_type qualifier_type);

struct ast_node *ast_node_new_gkcc_type_node(enum gkcc_type_type type);

struct ast_node *ast_node_new_ternary_node(struct ast_node *condition,
                                           struct ast_node *true_expr,
                                           struct ast_node *false_expr);

struct ast_node *ast_node_new_unary_node(enum ast_unary_type type,
                                         struct ast_node *of);

struct ast_node *ast_node_new_do_while_loop(struct ast_node *condition,
                                            struct ast_node *statements);

struct ast_node *ast_node_new_for_loop(struct ast_node *expr1,
                                       struct ast_node *expr2,
                                       struct ast_node *expr3,
                                       struct ast_node *statements);

struct ast_node *ast_node_new(enum ast_node_type node_type);

struct ast_node *ast_node_update_struct_or_union_specifier_node(
    struct ast_node *node, struct ast_node *ident, struct ast_node *members,
    struct gkcc_symbol_table_set *current_symbol_table, char *filename,
    int line_number);

struct ast_node *ast_node_new_struct_or_union_specifier_node(
    struct gkcc_symbol_table_set *current_symbol_table,
    enum gkcc_type_specifier_type type, struct ast_node *ident);

struct ast_node *ast_node_new_enum_definition_node(
    struct ast_node *ident, struct ast_node *enumerators);

struct ast_node *ast_node_apply_designator_to_all(struct ast_node *designators,
                                                  struct ast_node *initializer);

struct ast_node *ast_node_new_function_call_node(struct ast_node *function_name,
                                                 struct ast_node *parameters);

struct ast_node *ast_node_new_declaration_node(
    struct ast_node *declaration_specifiers,
    struct ast_node *init_declarator_list);

struct ast_node *ast_node_new_binop_node(enum ast_binop_type,
                                         struct ast_node *left,
                                         struct ast_node *right);

struct ast_node *ast_node_new_declaration_node_from_ident(
    struct ast_node *ident);

struct ast_node *ast_node_new_gkcc_array_type_node(struct ast_node *size);

struct ast_node *ast_node_new_gkcc_function_declarator_with_parameter_type_list(
    struct ast_node *parameter_type_list, struct ast_node *return_type);

struct ast_node *ast_node_new_member_access_node(
    struct gkcc_symbol_table_set *symbol_table_set,
    struct ast_node *struct_or_union, struct ast_node *member_identifier);

struct ast_node *ast_node_new_goto_node(struct ast_node *ident);

struct ast_node *ast_node_new_function_return(struct ast_node *to_return);

struct ast_node *ast_node_new_switch_case_case_node(
    struct ast_node *constant_expression, struct ast_node *statement);

struct ast_node *ast_node_new_switch_case_switch_node(
    struct ast_node *expression, struct ast_node *statements);
#endif  // GKCC_AST_CONSTRUCTORS_H

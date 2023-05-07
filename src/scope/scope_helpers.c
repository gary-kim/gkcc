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

#include "scope_helpers.h"

#include "misc/misc.h"
#include "scope/scope.h"

// declaration_list should be of type AST_NODE_LIST
enum gkcc_error gkcc_scope_add_variable_to_scope(
    struct gkcc_symbol_table_set *symbol_table_set,
    struct ast_node *declaration_list, int line_number, char *file_name) {
  gkcc_assert(declaration_list->type == AST_NODE_LIST,
              GKCC_ERROR_INVALID_ARGUMENTS,
              "gkcc_scope_add_variable_to_scope got a declaration_list that is "
              "not of typpe AST_NODE_LIST");

  for (struct ast_node *l = declaration_list; l != NULL; l = l->list.next) {
    struct ast_node *decl = l->list.node;

    // Declaration does not have an identifier. We can just not add it to the
    // symbol table in this case.
    if (decl->declaration.identifier == NULL) {
      continue;
    }

    // TODO: Other storage classes exist
    enum gkcc_storage_class storage_class =
        (symbol_table_set->scope == GKCC_SCOPE_GLOBAL)
            ? GKCC_STORAGE_CLASS_EXTERN
            : GKCC_STORAGE_CLASS_AUTO;
    struct gkcc_symbol *symbol = gkcc_symbol_new(
        decl->declaration.identifier->ident.name, storage_class,
        decl->declaration.type->gkcc_type.gkcc_type, line_number, file_name);

    // Add pointer back to the gkcc_symbol
    decl->declaration.identifier->ident.symbol_table_entry = symbol;

    enum gkcc_error error = gkcc_symbol_table_set_add_symbol(
        symbol_table_set, GKCC_NAMESPACE_GENERAL, symbol);
    if (error != GKCC_ERROR_SUCCESS) {
      if (error != GKCC_ERROR_REDECLARATION) {
        gkcc_error_fatal(GKCC_ERROR_REDECLARATION,
                         "Attempt to redeclare a variable. Dying");
      }
      return error;
    }
  }
  return GKCC_ERROR_SUCCESS;
}

enum gkcc_error gkcc_scope_add_tag_to_scope(
    struct gkcc_symbol_table_set *symbol_table_set,
    struct ast_node *struct_or_union_gkcc_type, char *filename,
    int line_number) {
  gkcc_assert(struct_or_union_gkcc_type->type == AST_NODE_GKCC_TYPE,
              GKCC_ERROR_INVALID_ARGUMENTS,
              "gkcc_scope_add_tag_to_scope() got a struct_or_union_declaration "
              "that is not of type AST_NODE_GKCC_TYPE");

  // Declaration does not have an identifier. We do not add it to the symbol
  // table in this case
  if (struct_or_union_gkcc_type->gkcc_type.gkcc_type->ident == NULL) {
    return GKCC_ERROR_SUCCESS;
  }

  struct gkcc_symbol *symbol = gkcc_symbol_new(
      struct_or_union_gkcc_type->gkcc_type.gkcc_type->ident->ident.name,
      GKCC_STORAGE_CLASS_INVALID,
      struct_or_union_gkcc_type->gkcc_type.gkcc_type, line_number, filename);

  struct_or_union_gkcc_type->gkcc_type.gkcc_type->ident->ident
      .symbol_table_entry = symbol;

  enum gkcc_error error = gkcc_symbol_table_set_add_symbol(
      symbol_table_set, GKCC_NAMESPACE_TAG, symbol);

  if (error == GKCC_ERROR_REDEFINITION) {
    gkcc_error_fatal(GKCC_ERROR_REDECLARATION,
                     "Attempt to redeclare a variable. Dying");
  }
  return error;
}

struct gkcc_symbol_table_set *gkcc_symbol_table_set_get_parent_symbol_table_set(
    struct gkcc_symbol_table_set *symbol_table_set) {
  gkcc_assert(symbol_table_set->parent_scope != NULL,
              GKCC_ERROR_INVALID_ARGUMENTS,
              "gkcc_symbol_table_set_get_parent_symbol_table_set() detected an "
              "attempt to get the parent symbol table set of a symbol table "
              "set that does not have a parent.");
  return symbol_table_set->parent_scope;
}
struct gkcc_symbol_table_set *
gkcc_symbol_table_set_get_symbol_table_set_of_struct_or_union_node(
    struct ast_node *node) {
  gkcc_assert(node->type == AST_NODE_GKCC_TYPE &&
                  node->gkcc_type.gkcc_type->type == GKCC_TYPE_TYPE_SPECIFIER &&
                  (node->gkcc_type.gkcc_type->type_specifier.type ==
                       GKCC_TYPE_SPECIFIER_STRUCT ||
                   node->gkcc_type.gkcc_type->type_specifier.type ==
                       GKCC_TYPE_SPECIFIER_UNION),
              GKCC_ERROR_INVALID_ARGUMENTS,
              "gkcc_symbol_table_set_get_symbol_table_set_of_struct_or_union_"
              "node() got node that "
              "is not AST_NODE_STRUCT_OR_UNION_SPECIFIER");

  return node->gkcc_type.gkcc_type->symbol_table_set;
}

enum gkcc_error gkcc_scope_add_label_to_scope(
    struct gkcc_symbol_table_set *symbol_table_set, struct ast_node *goto_node,
    struct ast_node *pointing_at, char *filename, int line_number) {


  char *label_name = goto_node->goto_node.ident->ident.name;
  // Does the symbol already exist can we point to it?
  struct gkcc_symbol *symbol = gkcc_symbol_table_set_get_symbol(
      symbol_table_set, label_name, GKCC_NAMESPACE_LABEL, true);

  if (symbol != NULL) {
    gkcc_assert(pointing_at == NULL || !symbol->fully_defined, GKCC_ERROR_REDEFINITION,
                "Attempt to put the same label twice in the same function. "
                "This is illegal. I am dying.");
    goto_node->goto_node.ident->ident.symbol_table_entry = symbol;
    goto_node->goto_node.symbol = symbol;
    return GKCC_ERROR_SUCCESS;
  }

  symbol = gkcc_symbol_new(label_name, GKCC_STORAGE_CLASS_INVALID, NULL,
                           line_number, filename);

  if (pointing_at != NULL) {
    symbol->fully_defined = true;
    symbol->location_ast = pointing_at;
  }

  return gkcc_symbol_table_set_add_symbol(symbol_table_set,
                                          GKCC_NAMESPACE_LABEL, symbol);
}

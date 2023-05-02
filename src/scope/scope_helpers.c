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
    struct gkcc_symbol *symbol =
        gkcc_symbol_new(decl->declaration.identifier->ident.name, storage_class,
                        line_number, file_name);

    // Add pointer back to the gkcc_symbol
    decl->declaration.symbol_table_entry = symbol;

    enum gkcc_error error =  gkcc_symbol_table_set_add_symbol(symbol_table_set, GKCC_NAMESPACE_GENERAL, symbol);
    if (error != GKCC_ERROR_SUCCESS) {
      return error;
    }
  }
  return GKCC_ERROR_SUCCESS;
}

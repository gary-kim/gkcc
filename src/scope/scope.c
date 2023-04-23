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

#include "scope.h"

#include <malloc.h>
#include <stdbool.h>
#include <string.h>

struct gkcc_symbol_table_set *gkcc_symbol_table_set_new(
    struct gkcc_symbol_table_set *parent, enum gkcc_scope scope) {
  struct gkcc_symbol_table_set *table =
      malloc(sizeof(struct gkcc_symbol_table_set));

  struct gkcc_symbol_table *new_tables =
      calloc(4, sizeof(struct gkcc_symbol_table));
  table->general_namespace = &new_tables[0];
  table->label_namespace = &new_tables[1];
  table->mini_namespace = &new_tables[2];
  table->tag_namespace = &new_tables[3];

  table->parent_scope = parent;
  table->scope = scope;
  return table;
}

struct gkcc_symbol_table *gkcc_symbol_table_set_get_symbol_table(
    struct gkcc_symbol_table_set *table_set, enum gkcc_namespace namespace) {
  switch (namespace) {
    case GKCC_NAMESPACE_GENERAL:
      return table_set->general_namespace;
    case GKCC_NAMESPACE_LABEL:
      return table_set->label_namespace;
    case GKCC_NAMESPACE_TAG:
      return table_set->tag_namespace;
    case GKCC_NAMESPACE_MINI:
      return table_set->mini_namespace;
  }
}

// This is not the function you are looking for.
// Take a look at gkcc_symbol_table_set_add_symbol instead.
// gkcc_symbol_table_add_symbol adds the given symbol to the given symbol_table
enum gkcc_error gkcc_symbol_table_add_symbol(struct gkcc_symbol_table *table,
                                             struct gkcc_symbol *symbol) {
  struct gkcc_symbol *last_symbol = table->symbol_list;

  // Shortcut for the first symbol in the symbol table
  if (last_symbol == NULL) {
    table->symbol_list = symbol;
    return GKCC_ERROR_SUCCESS;
  }

  // Check that symbol does not already exist in the given namespace
  if (gkcc_symbol_table_get_symbol(table, symbol->symbol_name) != NULL)
    return GKCC_ERROR_SYMBOL_ALREADY_EXISTS;

  // Go to last symbol in list
  for (; last_symbol->next != NULL; last_symbol = last_symbol->next);

  // Set the next symbol as our desired symbol
  last_symbol->next = symbol;

  return GKCC_ERROR_SUCCESS;
}

enum gkcc_error gkcc_symbol_table_set_add_symbol(
    struct gkcc_symbol_table_set *symbol_table_set,
    enum gkcc_namespace namespace, struct gkcc_symbol *symbol) {
  // Labels only get installed in function scope symbol tables
  if (namespace == GKCC_NAMESPACE_LABEL &&
      symbol_table_set->scope != GKCC_SCOPE_FUNCTION) {
    gkcc_assert(symbol_table_set->parent_scope != NULL,
                GKCC_ERROR_UNEXPECTED_NULL_VALUE,
                "Somehow got a label to add to a scope that does not have a "
                "parent. This should not happen");
    return gkcc_symbol_table_set_add_symbol(symbol_table_set->parent_scope,
                                            namespace, symbol);
  }

  struct gkcc_symbol_table *st =
      gkcc_symbol_table_set_get_symbol_table(symbol_table_set, namespace);

  return gkcc_symbol_table_add_symbol(st, symbol);
}

// gkcc_symbol_new returns a new gkcc_symbol
//
// the name need not be allocated forever as it will be strcpy(ed)
// at creation.
struct gkcc_symbol *gkcc_symbol_new(char *name,
                                    enum gkcc_storage_class storage_class) {
  struct gkcc_symbol *symbol = malloc(sizeof(struct gkcc_symbol));

  symbol->storage_class = storage_class;
  symbol->next = NULL;

  symbol->symbol_name = malloc(strlen(name));
  strcpy(symbol->symbol_name, name);

  return symbol;
}

// Gets a struct gkcc_symbol from the symbol table.
// Will not recurse up to the parent scope.
// Needs to be implemented separately from
// gkcc_symbol_table_set_get_symbol as that needs to be recursive
// but this cannot be recursive.
struct gkcc_symbol *gkcc_symbol_table_get_symbol(
    struct gkcc_symbol_table *symbol_table, char *name) {
  for (struct gkcc_symbol *symbol = symbol_table->symbol_list;
       symbol != NULL; symbol = symbol->next) {
    if (strcmp(symbol->symbol_name, name) == 0) {
      return symbol;
    }
  }

  return NULL;
}

// Gets a struct gkcc_symbol from the symbol table set in the given namespace.
// Will return null if symbol could not be found.
struct gkcc_symbol *gkcc_symbol_table_set_get_symbol(
    struct gkcc_symbol_table_set *symbol_table_set, char *name,
    enum gkcc_namespace namespace, bool recurse) {
  struct gkcc_symbol_table *symbol_table =
      gkcc_symbol_table_set_get_symbol_table(symbol_table_set, namespace);
  for (struct gkcc_symbol *symbol = symbol_table->symbol_list; symbol != NULL;
       symbol = symbol->next) {
    if (strcmp(symbol->symbol_name, name) == 0) {
      return symbol;
    }
  }
  if (recurse && symbol_table_set->parent_scope != NULL) {
    return gkcc_symbol_table_set_get_symbol(symbol_table_set->parent_scope,
                                            name, namespace, recurse);
  }
  return NULL;
}

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

#ifndef GKCC_SCOPE_H
#define GKCC_SCOPE_H

#include <stdbool.h>

#include "misc.h"
#include "types.h"

// === gkcc_namespace ===

#define ENUM_GKCC_NAMESPACE(GEN) \
  GEN(GKCC_NAMESPACE_GENERAL)    \
  GEN(GKCC_NAMESPACE_LABEL)      \
  GEN(GKCC_NAMESPACE_TAG)        \
  GEN(GKCC_NAMESPACE_MINI)

enum gkcc_namespace { ENUM_GKCC_NAMESPACE(ENUM_VALUES) };

static const char *const GKCC_NAMESPACE_STRING[] = {
    ENUM_GKCC_NAMESPACE(ENUM_STRINGS)};

#undef ENUM_GKCC_NAMESPACE

// === gkcc_scope ===
#define ENUM_GKCC_SCOPE(GEN) \
  GEN(GKCC_SCOPE_FUNCTION)   \
  GEN(GKCC_SCOPE_PROTOTYPE)  \
  GEN(GKCC_SCOPE_BLOCK)      \
  GEN(GKCC_SCOPE_GLOBAL)     \
  GEN(GKCC_SCOPE_STRUCT_OR_UNION)

enum gkcc_scope { ENUM_GKCC_SCOPE(ENUM_VALUES) };

static const char *const GKCC_SCOPE_STRING[] = {ENUM_GKCC_SCOPE(ENUM_STRINGS)};

#undef ENUM_GKCC_SCOPE

// === gkcc_storage_class ===
#define ENUM_GKCC_STORAGE_CLASS(GEN)      \
  GEN(GKCC_STORAGE_CLASS_INVALID)         \
  GEN(GKCC_STORAGE_CLASS_AUTO)            \
  GEN(GKCC_STORAGE_CLASS_REGISTER)        \
  GEN(GKCC_STORAGE_CLASS_EXTERN)          \
  GEN(GKCC_STORAGE_CLASS_EXTERN_EXPLICIT) \
  GEN(GKCC_STORAGE_CLASS_STATIC)

enum gkcc_storage_class { ENUM_GKCC_STORAGE_CLASS(ENUM_VALUES) };

static const char *const GKCC_STORAGE_CLASS_STRING[] = {
    ENUM_GKCC_STORAGE_CLASS(ENUM_STRINGS)};

#undef ENUM_GKCC_STORAGE_CLASS

// ================================
// === struct gkcc_symbol_table ===
// ================================

struct gkcc_symbol_table {
  struct gkcc_symbol *symbol_list;
};

// ====================================
// === struct gkcc_symbol_table_set ===
// ====================================

struct gkcc_symbol_table_set {
  enum gkcc_scope scope;
  struct gkcc_symbol_table *general_namespace;
  struct gkcc_symbol_table *label_namespace;
  struct gkcc_symbol_table *tag_namespace;
  struct gkcc_symbol_table *mini_namespace;
  struct gkcc_symbol_table_set *parent_scope;
};

// ===================
// === gkcc_symbol ===
// ===================

struct gkcc_symbol {
  char *symbol_name;
  enum gkcc_storage_class storage_class;
  struct gkcc_type *symbol_type;
  bool fully_defined;
  int effective_line_number;
  char *filename;

  struct gkcc_symbol *next;
  struct gkcc_symbol_table_set *symbol_table_set;
};

struct gkcc_symbol_table_set *gkcc_symbol_table_set_new(
    struct gkcc_symbol_table_set *parent, enum gkcc_scope scope);

struct gkcc_symbol *gkcc_symbol_new(char *name,
                                    enum gkcc_storage_class storage_class,
                                    struct gkcc_type *type, int line_number,
                                    char *filename);

struct gkcc_symbol_table *gkcc_symbol_table_set_get_symbol_table(
    struct gkcc_symbol_table_set *table_set, enum gkcc_namespace namespace);

enum gkcc_error gkcc_symbol_table_add_symbol(struct gkcc_symbol_table *table,
                                             struct gkcc_symbol *symbol);

struct gkcc_symbol *gkcc_symbol_table_set_get_symbol(
    struct gkcc_symbol_table_set *symbol_table_set, char *name,
    enum gkcc_namespace namespace, bool recurse);

struct gkcc_symbol *gkcc_symbol_table_get_symbol(
    struct gkcc_symbol_table *symbol_table, char *name);

enum gkcc_error gkcc_symbol_table_set_add_symbol(
    struct gkcc_symbol_table_set *symbol_table_set,
    enum gkcc_namespace namespace, struct gkcc_symbol *symbol);

void gkcc_symbol_print_string(struct gkcc_symbol *symbol, int depth);

void gkcc_symbol_table_print(struct gkcc_symbol_table *symbol_table,
                             int depth);
#endif  // GKCC_SCOPE_H

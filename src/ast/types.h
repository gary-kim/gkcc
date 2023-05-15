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

#ifndef GKCC_TYPES_H
#define GKCC_TYPES_H

#include "ast.h"
#include "misc.h"
#include "scope.h"

// =============================
// === struct gkcc_qualifier ===
// =============================

#define ENUM_GKCC_QUALIFIER_TYPE(GEN) \
  GEN(GKCC_QUALIFIER_CONST)           \
  GEN(GKCC_QUALIFIER_VOLATILE)        \
  GEN(GKCC_QUALIFIER_RESTRICT)

enum gkcc_qualifier_type { ENUM_GKCC_QUALIFIER_TYPE(ENUM_VALUES) };
static const char* const GKCC_QUALIFIER_TYPE_STRING[] = {
    ENUM_GKCC_QUALIFIER_TYPE(ENUM_STRINGS)};
#undef ENUM_GKCC_QUALIFIER_TYPE

struct gkcc_qualifier {
  enum gkcc_qualifier_type type;
  struct gkcc_type* of;
};

// =============================
// === struct gkcc_storage_specifier ===
// =============================

#define ENUM_GKCC_STORAGE_CLASS_SPECIFIER_TYPE(GEN) \
  GEN(GKCC_STORAGE_CLASS_SPECIFIER_TYPEDEF)         \
  GEN(GKCC_STORAGE_CLASS_SPECIFIER_EXTERN)          \
  GEN(GKCC_STORAGE_CLASS_SPECIFIER_STATIC)          \
  GEN(GKCC_STORAGE_CLASS_SPECIFIER_THREAD_LOCAL)    \
  GEN(GKCC_STORAGE_CLASS_SPECIFIER_AUTO)            \
  GEN(GKCC_STORAGE_CLASS_SPECIFIER_REGISTER)

enum gkcc_storage_class_specifier_type {
  ENUM_GKCC_STORAGE_CLASS_SPECIFIER_TYPE(ENUM_VALUES)
};

static const char* const ENUM_GKCC_STORAGE_CLASS_SPECIFICER_TYPE_STRING[] = {
    ENUM_GKCC_STORAGE_CLASS_SPECIFIER_TYPE(ENUM_STRINGS)};

struct gkcc_storage_class_specifier {
  enum gkcc_storage_class_specifier_type type;
  struct gkcc_type* of;
};

#undef ENUM_GKCC_STORAGE_CLASS_SPECIFIER_TYPE

// ==================================
// === struct gkcc_type_specifier ===
// ==================================

#define ENUM_GKCC_TYPE_SPECIFIER_TYPE(GEN) \
  GEN(GKCC_TYPE_SPECIFIER_VOID)            \
  GEN(GKCC_TYPE_SPECIFIER_CHAR)            \
  GEN(GKCC_TYPE_SPECIFIER_SHORT)           \
  GEN(GKCC_TYPE_SPECIFIER_INT)             \
  GEN(GKCC_TYPE_SPECIFIER_LONG)            \
  GEN(GKCC_TYPE_SPECIFIER_FLOAT)           \
  GEN(GKCC_TYPE_SPECIFIER_DOUBLE)          \
  GEN(GKCC_TYPE_SPECIFIER_SIGNED)          \
  GEN(GKCC_TYPE_SPECIFIER_UNSIGNED)        \
  GEN(GKCC_TYPE_SPECIFIER_BOOL)            \
  GEN(GKCC_TYPE_SPECIFIER_COMPLEX)         \
  GEN(GKCC_TYPE_SPECIFIER_STRUCT)          \
  GEN(GKCC_TYPE_SPECIFIER_UNION)           \
  GEN(GKCC_TYPE_SPECIFIER_ENUM)

enum gkcc_type_specifier_type { ENUM_GKCC_TYPE_SPECIFIER_TYPE(ENUM_VALUES) };

static const char* const GKCC_TYPE_SPECIFIER_TYPE_STRING[] = {
    ENUM_GKCC_TYPE_SPECIFIER_TYPE(ENUM_STRINGS)};

struct gkcc_type_specifier {
  enum gkcc_type_specifier_type type;
  struct ast_node* ident;
  struct gkcc_type* of;
};

#undef ENUM_GKCC_TYPE_SPECIFIER_TYPE

// ==============================
// === struct gkcc_array ===
// ==============================

struct gkcc_array {
  struct ast_node* size;
};

// =============================
// === struct gkcc_type_list ===
// =============================

struct gkcc_type_list {
  struct gkcc_type* type;
  struct gkcc_type_list* next;
};

// ========================================
// === struct gkcc_function_declaration ===
// ========================================

struct gkcc_function_declaration {
  struct ast_node* parameters;
  struct gkcc_type* return_type;
  struct ast_node* statements;
};

// ========================
// === struct gkcc_type ===
// ========================

#define ENUM_GKCC_TYPE_TYPE(GEN)         \
  GEN(GKCC_TYPE_UNKNOWN)                 \
  GEN(GKCC_TYPE_FUNCTION)                \
  GEN(GKCC_TYPE_PTR)                     \
  GEN(GKCC_TYPE_ARRAY)                   \
  GEN(GKCC_TYPE_STRUCT)                  \
  GEN(GKCC_TYPE_UNION)                   \
  GEN(GKCC_TYPE_ENUM)                    \
  GEN(GKCC_TYPE_QUALIFIER)               \
  GEN(GKCC_TYPE_STORAGE_CLASS_SPECIFIER) \
  GEN(GKCC_TYPE_TYPE_SPECIFIER)          \
  GEN(GKCC_TYPE_SCALAR_LONGLONG)         \
  GEN(GKCC_TYPE_SCALAR_LONG_DOUBLE)      \
  GEN(GKCC_TYPE_SCALAR_DOUBLE)           \
  GEN(GKCC_TYPE_SCALAR_FLOAT)            \
  GEN(GKCC_TYPE_SCALAR_LONG)             \
  GEN(GKCC_TYPE_SCALAR_INT)              \
  GEN(GKCC_TYPE_SCALAR_CHAR)             \
  GEN(GKCC_TYPE_SCALAR_VOID)             \
  GEN(GKCC_TYPE_SCALAR_SHORT)            \
  GEN(GKCC_TYPE_SIGNED)                  \
  GEN(GKCC_TYPE_UNSIGNED)

enum gkcc_type_type { ENUM_GKCC_TYPE_TYPE(ENUM_VALUES) };

static const char* const GKCC_TYPE_TYPE_STRING[] = {
    ENUM_GKCC_TYPE_TYPE(ENUM_STRINGS)};

#undef ENUM_GKCC_TYPE_TYPE

struct gkcc_type {
  enum gkcc_type_type type;
  union {
    struct gkcc_qualifier qualifier;
    struct gkcc_storage_class_specifier storage_class_specifier;
    struct gkcc_type_specifier type_specifier;
    struct gkcc_array array;
    struct gkcc_function_declaration function_declaration;
  };
  struct gkcc_type* of;
  struct ast_node* ident;
  struct gkcc_symbol_table_set* symbol_table_set;
};

// =============================
// === Function Declarations ===
// =============================

struct gkcc_type* gkcc_type_new(enum gkcc_type_type type);
struct gkcc_type* gkcc_type_append(struct gkcc_type* parent,
                                   struct gkcc_type* child);
bool gkcc_is_gkcc_type_scalar(struct gkcc_type* gkcc_type);
#endif  // GKCC_TYPES_H

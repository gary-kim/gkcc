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

#include "misc.h"

// ==========================
// === struct gkcc_scalar ===
// ==========================

#define ENUM_GKCC_SCALAR_TYPE(GEN) \
  GEN(GKCC_SCALAR_LONGLONG)        \
  GEN(GKCC_SCALAR_LONG_DOUBLE)     \
  GEN(GKCC_SCALAR_DOUBLE)          \
  GEN(GKCC_SCALAR_FLOAT)           \
  GEN(GKCC_SCALAR_LONG)            \
  GEN(GKCC_SCALAR_INT)             \
  GEN(GKCC_SCALAR_CHAR)

enum gkcc_scalar_type { ENUM_GKCC_SCALAR_TYPE(ENUM_VALUES) };

static const char* const GKCC_SCALAR_TYPE_STRING[] = {
    ENUM_GKCC_SCALAR_TYPE(ENUM_STRINGS)};

#undef ENUM_GKCC_SCALAR_TYPE

struct gkcc_scalar {
  enum gkcc_scalar_type type;
};

// =============================
// === struct gkcc_qualifier ===
// =============================

#define ENUM_GKCC_QUALIFIER_TYPE(GEN) \
  GEN(GKCC_QUALIFIER_CONST)           \
  GEN(GKCC_QUALIFIER_VOLATILE)        \
  GEN(GKCC_QUALIFIER_RESTRICT)

enum gkcc_qualifier_type {
  ENUM_GKCC_QUALIFIER_TYPE(ENUM_VALUES)
};
static const char * const GKCC_QUALIFIER_TYPE_STRING[] = {
    ENUM_GKCC_QUALIFIER_TYPE(ENUM_STRINGS)
};
#undef ENUM_GKCC_QUALIFIER_TYPE

struct gkcc_qualifier {
  enum gkcc_qualifier_type type;
  struct gkcc_type *of;
};


#define ENUM_GKCC_TYPE_TYPE(GEN) \
  GEN(GKCC_TYPE_SCALAR)          \
  GEN(GKCC_TYPE_FUNCTION)        \
  GEN(GKCC_TYPE_PTR)             \
  GEN(GKCC_TYPE_ARRAY)           \
  GEN(GKCC_TYPE_STRUCT)          \
  GEN(GKCC_TYPE_UNION)           \
  GEN(GKCC_TYPE_QUALIFIER)

enum gkcc_type_type { ENUM_GKCC_TYPE_TYPE(ENUM_VALUES) };

static const char* const GKCC_TYPE_TYPE_STRING[] = {
    ENUM_GKCC_TYPE_TYPE(ENUM_STRINGS)};

#undef ENUM_GKCC_TYPE_TYPE

// ========================
// === struct gkcc_type ===
// ========================

struct gkcc_type {
  enum gkcc_type_type type;
  union {
    struct gkcc_scalar scalar;
    struct gkcc_qualifier qualifier;
  };
};

#endif  // GKCC_TYPES_H

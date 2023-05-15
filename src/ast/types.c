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

#include "types.h"

#include <malloc.h>
#include <memory.h>

struct gkcc_type* gkcc_type_new(enum gkcc_type_type type) {
  struct gkcc_type* gkcc_type = malloc(sizeof(struct gkcc_type));
  memset(gkcc_type, 0, sizeof(struct gkcc_type));
  gkcc_type->type = type;
  return gkcc_type;
}

struct gkcc_type* gkcc_type_append(struct gkcc_type* parent,
                                   struct gkcc_type* child) {
  // Null appends are noop
  if (child == NULL) return parent;
  if (parent == NULL) return child;

  struct gkcc_type* last_node = parent;
  while (last_node->of != NULL) {
    last_node = last_node->of;
  }

  last_node->of = child;
  return parent;
}

bool gkcc_is_gkcc_type_scalar(struct gkcc_type* gkcc_type) {
  enum gkcc_type_type type = gkcc_type->type;
  switch (type) {
    case GKCC_TYPE_SCALAR_LONGLONG:
    case GKCC_TYPE_SCALAR_LONG_DOUBLE:
    case GKCC_TYPE_SCALAR_DOUBLE:
    case GKCC_TYPE_SCALAR_FLOAT:
    case GKCC_TYPE_SCALAR_LONG:
    case GKCC_TYPE_SCALAR_INT:
    case GKCC_TYPE_SCALAR_CHAR:
    case GKCC_TYPE_SCALAR_VOID:
    case GKCC_TYPE_SCALAR_SHORT:
    case GKCC_TYPE_ENUM:
    case GKCC_TYPE_SIGNED:
    case GKCC_TYPE_UNSIGNED:
      return true;
    default:
      return false;
  }
  return false;
}

struct gkcc_type* gkcc_type_new_signed_int_type(void) {
  struct gkcc_type* integer_type = gkcc_type_new(GKCC_TYPE_SCALAR_INT);
  struct gkcc_type* signed_type = gkcc_type_new(GKCC_TYPE_SIGNED);
  signed_type->of = integer_type;
  return signed_type;
}

int gkcc_type_sizeof(struct gkcc_type* type) {
  // TODO: Make everything not an integer
  // For now, EVERYTHING IS AN INT
  if (type == NULL) {
    return sizeof(int);
  }
  if (gkcc_is_gkcc_type_scalar(type)) {
    return sizeof(int);
  }
  switch (type->type) {
    case GKCC_TYPE_ARRAY:
      gkcc_assert(type->array.size->type == AST_NODE_CONSTANT,
                  GKCC_ERROR_NOT_YET_IMPLEMENTED,
                  "Non immediate constant array sizes are not yet supported");
      gkcc_assert(type->array.size->constant.type == AST_CONSTANT_INT,
                  GKCC_ERROR_NOT_YET_IMPLEMENTED,
                  "Non integer array sizes are not yet supported");
      return type->array.size->constant.yint * gkcc_type_sizeof(type->of);
    case GKCC_TYPE_STRUCT:
      gkcc_error_fatal(GKCC_ERROR_NOT_YET_IMPLEMENTED,
                       "sizeof(struct) support is not yet implemented");
      break;
    case GKCC_TYPE_UNION:
      gkcc_error_fatal(GKCC_ERROR_NOT_YET_IMPLEMENTED,
                       "sizeof(union) support is not yet implemented");
      break;
    default:
      break;
  }
  return sizeof(int);
}

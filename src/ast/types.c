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
      return true;
    default:
      return false;
  }
  return false;
}

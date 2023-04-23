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

  if (parent->of != NULL) {
    gkcc_type_append(parent->of, child);
    return parent;
  }
  parent->of = child;
  return parent;
}

enum gkcc_type_type gkcc_type_make_scalar_long(enum gkcc_type_type original) {
  switch (original) {
    case GKCC_TYPE_SCALAR_LONGLONG:
      break;
    case GKCC_TYPE_SCALAR_LONG_DOUBLE:
      break;
    case GKCC_TYPE_SCALAR_DOUBLE:
      break;
    case GKCC_TYPE_SCALAR_FLOAT:
      break;
    case GKCC_TYPE_SCALAR_LONG:
      break;
    case GKCC_TYPE_SCALAR_INT:
      break;
    case GKCC_TYPE_SCALAR_CHAR:
      break;
    case GKCC_TYPE_SCALAR_VOID:
      break;
    case GKCC_TYPE_SCALAR_SHORT:
      break;
  }
}

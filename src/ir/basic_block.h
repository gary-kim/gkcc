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

#ifndef GKCC_BASIC_BLOCK_H
#define GKCC_BASIC_BLOCK_H

#include "scope/scope.h"
#include "ir/quads.h"

void gkcc_basic_block_build_basic_blocks(struct ast_node *node);

// ===============================
// === struct gkcc_basic_block ===
// ===============================

struct gkcc_basic_block {
  struct gkcc_ir_quad_list* quads_in_bb;
  struct gkcc_ir_quad* comparison;
  struct gkcc_basic_block* true_branch;
  struct gkcc_basic_block* false_branch;
  struct gkcc_basic_block* next_block;
};

#endif  // GKCC_BASIC_BLOCK_H

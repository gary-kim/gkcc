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

#include "ir/quads.h"
#include "scope/scope.h"

// ===============================
// === struct gkcc_basic_block ===
// ===============================

struct gkcc_basic_block {
  char *bb_name;
  struct gkcc_ir_quad_list *quads_in_bb;
  struct gkcc_ir_quad *comparison;
  struct gkcc_basic_block *true_branch;
  struct gkcc_basic_block *false_branch;
};

// ======================================
// === struct gkcc_basic_block_status ===
// ======================================

struct gkcc_basic_block_status {
  struct gkcc_basic_block *thisBB;
  struct gkcc_basic_block_status *trueBB;
  struct gkcc_basic_block_status *falseBB;
  struct gkcc_basic_block_status *continueBB;
  struct gkcc_basic_block_status *breakBB;
};

struct gkcc_basic_block *gkcc_basic_block_new(
    struct gkcc_ir_generation_state *gen_state);

struct gkcc_basic_block_status *gkcc_basic_block_status_new(
    struct gkcc_ir_generation_state *gen_state,
    struct gkcc_basic_block_status *continueBB,
    struct gkcc_basic_block_status *breakBB,
    struct gkcc_basic_block_status *trueBB,
    struct gkcc_basic_block_status *falseBB);

void gkcc_internal_recurse_basic_blocks(
    struct gkcc_ir_generation_state *gen_state, struct ast_node *nodes,
    struct gkcc_basic_block_status *bb_status);

struct gkcc_ir_function *gkcc_internal_build_basic_blocks_for_function(
    struct gkcc_ir_generation_state *gen_state, struct ast_node *function_node);

void gkcc_basic_block_print(struct gkcc_basic_block *bb);

#endif  // GKCC_BASIC_BLOCK_H

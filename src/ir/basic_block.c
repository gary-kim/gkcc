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

#include "basic_block.h"

#include <malloc.h>
#include <memory.h>

#include "ir/translators.h"
#include "misc/misc.h"
#include "scope/scope.h"

void gkcc_internal_recurse_basic_blocks(
    struct gkcc_ir_generation_state *gen_state, struct ast_node *nodes,
    struct gkcc_basic_block_status *bb_status) {
  struct gkcc_ir_translation_result last_result;
  for (struct ast_node *lnode = nodes; lnode != NULL;
       lnode = lnode->list.next) {
    struct ast_node *tnode = lnode->list.node;
    struct gkcc_ir_translation_result translation_result;

    switch (tnode->type) {
      case AST_NODE_GOTO_NODE:
        break;
    }

    if (tnode->type == AST_NODE_FOR_LOOP) {
      struct gkcc_basic_block_status *next_BB_status =
          gkcc_basic_block_status_new(gen_state, bb_status->continueBB,
                                      bb_status->breakBB, bb_status->trueBB,
                                      bb_status->falseBB);
      struct gkcc_basic_block_status *body_BB_status =
          gkcc_basic_block_status_new(gen_state, NULL, NULL, NULL, NULL);
      struct gkcc_basic_block_status *cond_BB_status =
          gkcc_basic_block_status_new(gen_state, NULL, NULL, body_BB_status,
                                      next_BB_status);
      struct gkcc_basic_block_status *iter_BB_status =
          gkcc_basic_block_status_new(gen_state, NULL, NULL, cond_BB_status,
                                      cond_BB_status);
      struct gkcc_basic_block_status *init_BB_status =
          gkcc_basic_block_status_new(gen_state, NULL, NULL, cond_BB_status,
                                      cond_BB_status);

      body_BB_status->continueBB = iter_BB_status;
      body_BB_status->breakBB = next_BB_status;
      body_BB_status->trueBB = iter_BB_status;
      body_BB_status->falseBB = iter_BB_status;

      gkcc_internal_recurse_basic_blocks(gen_state, lnode->list.next,
                                         next_BB_status);
      gkcc_internal_recurse_basic_blocks(gen_state, tnode->for_loop.statements,
                                         body_BB_status);
      gkcc_internal_recurse_basic_blocks(gen_state, tnode->for_loop.expr3,
                                         iter_BB_status);
      gkcc_internal_recurse_basic_blocks(gen_state, tnode->for_loop.expr2,
                                         cond_BB_status);
      gkcc_internal_recurse_basic_blocks(gen_state, tnode->for_loop.expr1,
                                         init_BB_status);

      bb_status->trueBB = init_BB_status;
      bb_status->falseBB = init_BB_status;

      goto CLEANUP_AND_RETURN;
    }

    if (tnode->type == AST_NODE_IF_STATEMENT) {
      struct gkcc_basic_block_status *next_BB_status =
          gkcc_basic_block_status_new(gen_state, bb_status->continueBB,
                                      bb_status->breakBB, bb_status->trueBB,
                                      bb_status->falseBB);
      struct gkcc_basic_block_status *else_BB_status =
          gkcc_basic_block_status_new(gen_state, bb_status->continueBB,
                                      bb_status->breakBB, next_BB_status,
                                      next_BB_status);
      struct gkcc_basic_block_status *then_BB_status =
          gkcc_basic_block_status_new(gen_state, bb_status->continueBB,
                                      bb_status->breakBB, next_BB_status,
                                      next_BB_status);
      struct gkcc_basic_block_status *cond_BB_status =
          gkcc_basic_block_status_new(gen_state, NULL, NULL, then_BB_status,
                                      else_BB_status);

      gkcc_internal_recurse_basic_blocks(gen_state, lnode->list.next,
                                         next_BB_status);
      gkcc_internal_recurse_basic_blocks(
          gen_state, tnode->if_statement.then_statement, then_BB_status);
      gkcc_internal_recurse_basic_blocks(
          gen_state, tnode->if_statement.else_statement, else_BB_status);
      gkcc_internal_recurse_basic_blocks(
          gen_state, tnode->if_statement.condition, cond_BB_status);

      bb_status->trueBB = cond_BB_status;
      bb_status->falseBB = cond_BB_status;

      goto CLEANUP_AND_RETURN;
    }

    if (tnode->type == AST_NODE_JUMP_BREAK) {
      gkcc_assert(bb_status->breakBB != NULL, GKCC_ERROR_INVALID_CODE,
                  "Attempted to use 'break' when there is nowhere to break to");
      struct gkcc_ir_quad *jump_ir = gkcc_ir_quad_new_with_args(
          GKCC_IR_QUAD_INSTRUCTION_BRANCH, NULL,
          gkcc_ir_quad_register_new_basic_block(bb_status->breakBB->thisBB),
          NULL);
      translation_result.ir_quad_list = gkcc_ir_quad_list_append(NULL, jump_ir);
      goto TRANSLATION_COMPLETE;
    }

    if (tnode->type == AST_NODE_JUMP_CONTINUE) {
      gkcc_assert(bb_status->continueBB != NULL, GKCC_ERROR_INVALID_CODE,
                  "Attempted to use 'break' when there is nowhere to break to");
      struct gkcc_ir_quad *jump_ir = gkcc_ir_quad_new_with_args(
          GKCC_IR_QUAD_INSTRUCTION_BRANCH, NULL,
          gkcc_ir_quad_register_new_basic_block(bb_status->continueBB->thisBB),
          NULL);
      translation_result.ir_quad_list = gkcc_ir_quad_list_append(NULL, jump_ir);
      bb_status->trueBB = bb_status->continueBB;
      bb_status->falseBB = bb_status->continueBB;
      goto TRANSLATION_COMPLETE;
    }

    // This could be skipped past with a goto if something else does the
    // translation
    translation_result = gkcc_ir_quad_generate_for_ast(gen_state, tnode);

    bb_status->thisBB->quads_in_bb = gkcc_ir_quad_list_append_list(
        bb_status->thisBB->quads_in_bb, translation_result.ir_quad_list);
    last_result = translation_result;
    continue;

  TRANSLATION_COMPLETE:
    bb_status->thisBB->quads_in_bb = gkcc_ir_quad_list_append_list(
        bb_status->thisBB->quads_in_bb, translation_result.ir_quad_list);
    last_result = translation_result;
    break;
  }

CLEANUP_AND_RETURN:
  bb_status->thisBB->true_branch =
      bb_status->trueBB == NULL ? NULL : bb_status->trueBB->thisBB;
  bb_status->thisBB->false_branch =
      bb_status->falseBB == NULL ? NULL : bb_status->falseBB->thisBB;

  if (bb_status->thisBB->true_branch == NULL) {
    return;
  }

  // This optimization is creating some issues so commenting out for now
  //  if (bb_status->thisBB->quads_in_bb == NULL) {
  //    bb_status->trueBB = bb_status->trueBB->trueBB;
  //    bb_status->falseBB = bb_status->falseBB->falseBB;
  //    return;
  //  }

  if (bb_status->thisBB->true_branch == bb_status->thisBB->false_branch) {
    struct gkcc_ir_quad *jump_ir = gkcc_ir_quad_new_with_args(
        GKCC_IR_QUAD_INSTRUCTION_BRANCH, NULL,
        gkcc_ir_quad_register_new_basic_block(bb_status->thisBB->true_branch),
        NULL);
    bb_status->thisBB->quads_in_bb =
        gkcc_ir_quad_list_append(bb_status->thisBB->quads_in_bb, jump_ir);
    return;
  }

  struct gkcc_ir_quad *true_jump_ir = gkcc_ir_quad_new_with_args(
      GKCC_IR_QUAD_INSTRUCTION_BRANCH_IF_TRUE, NULL,
      gkcc_ir_quad_register_new_basic_block(bb_status->thisBB->true_branch),
      last_result.result);
  bb_status->thisBB->quads_in_bb =
      gkcc_ir_quad_list_append(bb_status->thisBB->quads_in_bb, true_jump_ir);

  struct gkcc_ir_quad *false_jump_ir = gkcc_ir_quad_new_with_args(
      GKCC_IR_QUAD_INSTRUCTION_BRANCH_IF_FALSE, NULL,
      gkcc_ir_quad_register_new_basic_block(bb_status->thisBB->false_branch),
      last_result.result);
  bb_status->thisBB->quads_in_bb =
      gkcc_ir_quad_list_append(bb_status->thisBB->quads_in_bb, false_jump_ir);

  return;
}

struct gkcc_ir_function *gkcc_internal_build_basic_blocks_for_function(
    struct gkcc_ir_generation_state *gen_state,
    struct ast_node *function_node) {
  struct gkcc_ir_function *ir_function =
      gkcc_ir_function_new(function_node->declaration.identifier->ident.name);
  gen_state->current_function = ir_function;
  struct ast_node *lnode = function_node->declaration.type->gkcc_type.gkcc_type
                               ->function_declaration.statements;
  struct gkcc_basic_block_status *bb_status =
      gkcc_basic_block_status_new(gen_state, NULL, NULL, NULL, NULL);

  gkcc_internal_recurse_basic_blocks(gen_state, lnode, bb_status);

  ir_function->entrance_basic_block = bb_status->thisBB;

  return ir_function;
}

struct gkcc_basic_block *gkcc_basic_block_new(
    struct gkcc_ir_generation_state *gen_state) {
  struct gkcc_basic_block *bb = malloc(sizeof(struct gkcc_basic_block));
  memset(bb, 0, sizeof(struct gkcc_basic_block));
  bb->bb_number = gen_state->current_basic_block_number++;
  char buf[(1 << 12) + 1];
  size_t len = sprintf(buf, ".BB.%d", bb->bb_number);
  bb->bb_name = malloc(len + 1);
  strcpy(bb->bb_name, buf);
  return bb;
}

struct gkcc_basic_block_status *gkcc_basic_block_status_new(
    struct gkcc_ir_generation_state *gen_state,
    struct gkcc_basic_block_status *continueBB,
    struct gkcc_basic_block_status *breakBB,
    struct gkcc_basic_block_status *trueBB,
    struct gkcc_basic_block_status *falseBB) {
  struct gkcc_basic_block_status *bb_status =
      malloc(sizeof(struct gkcc_basic_block_status));
  memset(bb_status, 0, sizeof(struct gkcc_basic_block_status));

  bb_status->thisBB = gkcc_basic_block_new(gen_state);
  bb_status->continueBB = continueBB;
  bb_status->breakBB = breakBB;
  bb_status->trueBB = trueBB;
  bb_status->falseBB = falseBB;

  return bb_status;
}

void gkcc_basic_block_print(bool *printed, struct gkcc_basic_block *bb) {
  if (bb == NULL) return;
  if (printed[bb->bb_number]) return;

  printed[bb->bb_number] = true;
  printf("%s:\n", bb->bb_name);
  gkcc_ir_quad_print(bb->quads_in_bb);
  gkcc_basic_block_print(printed, bb->true_branch);
  if (bb->true_branch != bb->false_branch)
    gkcc_basic_block_print(printed, bb->false_branch);
}

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

#include "ir/ir_full.h"

#include <malloc.h>
#include <memory.h>

#include "ir/basic_block.h"
#include "ir/quads.h"

struct gkcc_ir_full *gkcc_ir_build_full(struct ast_node *node) {
  gkcc_assert(node->type == AST_NODE_TOP_LEVEL, GKCC_ERROR_INVALID_ARGUMENTS,
              "gkcc_basic_block_build_basic_blocks() got a node "
              "that is not of type AST_NODE_TOP_LEVEL");

  struct gkcc_ir_generation_state *gen_state = gkcc_ir_generation_state_new();
  struct gkcc_ir_full *ir_full = gkcc_ir_full_new();

  // Go through and process all function definitions
  for (struct ast_node *lnode = node->top_level.list; lnode != NULL;
       lnode = lnode->list.next) {
    struct ast_node *tnode = lnode->list.node;

    // Skip anything that is not a function declaration
    if (tnode->type != AST_NODE_DECLARATION ||
        tnode->declaration.type->gkcc_type.gkcc_type->type !=
            GKCC_TYPE_FUNCTION) {
      continue;
    }

    struct gkcc_ir_function *fn =
        gkcc_internal_build_basic_blocks_for_function(gen_state, tnode);
    ir_full->function_list =
        gkcc_ir_function_list_append(ir_full->function_list, fn);
  }

  return ir_full;
}

struct gkcc_ir_full *gkcc_ir_full_new(void) {
  struct gkcc_ir_full *ir_full = malloc(sizeof(struct gkcc_ir_full));
  memset(ir_full, 0, sizeof(struct gkcc_ir_full));

  return ir_full;
}
struct gkcc_ir_function_list *gkcc_ir_function_list_append(
    struct gkcc_ir_function_list *fn_list, struct gkcc_ir_function *fn) {
  if (fn_list == NULL) {
    return gkcc_ir_function_list_new(fn);
  }

  struct gkcc_ir_function_list *end;
  for (end = fn_list; fn_list != NULL; fn_list = fn_list->next)
    ;
  end->next = gkcc_ir_function_list_new(fn);

  return fn_list;
}
struct gkcc_ir_function_list *gkcc_ir_function_list_new(
    struct gkcc_ir_function *fn) {
  struct gkcc_ir_function_list *fn_list =
      malloc(sizeof(struct gkcc_ir_function_list));
  memset(fn_list, 0, sizeof(struct gkcc_ir_function_list));

  fn_list->fn = fn;

  return fn_list;
}

void gkcc_ir_full_print(struct gkcc_ir_full *ir_full) {
  // Print all functions
  for (struct gkcc_ir_function_list *fn_list = ir_full->function_list;
       fn_list != NULL; fn_list = fn_list->next) {
    struct gkcc_ir_function *fn = fn_list->fn;
    printf("FN_%s:\n", fn->function_name);

    // Print all BBs
    gkcc_basic_block_print(fn->entrance_basic_block);
  }
}

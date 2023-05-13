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

#include "misc/misc.h"
#include "scope/scope.h"

void gkcc_internal_build_basic_block_from_list

void gkcc_internal_build_basic_block(struct ast_node *function_node) {
  function_node->declaration.type->gkcc_type.gkcc_type->function_declaration
      .statements;
  for (struct ast_node *lnode = function_node->declaration.type->gkcc_type
                                    .gkcc_type->function_declaration.statements;
       lnode != NULL; lnode = lnode->list.next) {
    struct ast_node *tnode = lnode->list.node;

    if (tnode !=)
  }
}

void gkcc_basic_block_build_basic_blocks(struct ast_node *node) {
  gkcc_assert(node->type == AST_NODE_TOP_LEVEL, GKCC_ERROR_INVALID_ARGUMENTS,
              "gkcc_basic_block_build_basic_blocks() got a node "
              "that is not of type AST_NODE_TOP_LEVEL");

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

    gkcc_internal_build_basic_block(tnode);
  }
}

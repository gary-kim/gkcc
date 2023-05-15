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
  ir_full->gen_state = gen_state;
  gen_state->ir_full = ir_full;

  // Go through and process all function definitions
  for (struct ast_node *lnode = node->top_level.list; lnode != NULL;
       lnode = lnode->list.next) {
    struct ast_node *tnode = lnode->list.node;

    // Skip anything that is not a declaration
    if (tnode->type != AST_NODE_DECLARATION) {
      continue;
    }

    // Handles functions
    if (tnode->declaration.type->gkcc_type.gkcc_type->type ==
        GKCC_TYPE_FUNCTION) {
      // Skip functions without a definition
      if (tnode->declaration.type->gkcc_type.gkcc_type->function_declaration
              .statements == NULL) {
        continue;
      }
      struct gkcc_ir_function *fn =
          gkcc_internal_build_basic_blocks_for_function(gen_state, tnode);
      ir_full->function_list =
          gkcc_ir_function_list_append(ir_full->function_list, fn);
      continue;
    }

    // Handle variable declarations

    ir_full->global_symbols = gkcc_ir_symbol_list_append(
        ir_full->global_symbols,
        gkcc_ir_symbol_new(
            tnode->declaration.identifier->ident.symbol_table_entry, false));
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
  for (end = fn_list; end->next != NULL; end = end->next)
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

struct gkcc_ir_symbol_list *gkcc_ir_symbol_list_append(
    struct gkcc_ir_symbol_list *list, struct gkcc_ir_symbol *symbol) {
  if (list == NULL) {
    return gkcc_ir_symbol_list_new(symbol);
  }

  struct gkcc_ir_symbol_list *end;
  for (end = list; end->next != NULL; end = end->next)
    ;

  end->next = gkcc_ir_symbol_list_new(symbol);

  return list;
}

struct gkcc_ir_symbol_list *gkcc_ir_symbol_list_new(
    struct gkcc_ir_symbol *symbol) {
  struct gkcc_ir_symbol_list *list = malloc(sizeof(struct gkcc_ir_symbol_list));
  memset(list, 0, sizeof(struct gkcc_ir_symbol_list));

  list->symbol = symbol;

  return list;
}

struct gkcc_ir_symbol *gkcc_ir_symbol_new(struct gkcc_symbol *gs,
                                          bool is_global) {
  struct gkcc_ir_symbol *is = malloc(sizeof(struct gkcc_ir_symbol));
  memset(is, 0, sizeof(struct gkcc_ir_symbol));

  is->symbol = gs;
  is->is_global = is_global;

  return is;
}

void gkcc_ir_full_print(struct gkcc_ir_full *ir_full) {
  // List of printed basic blocks
  bool printed_bbs[ir_full->gen_state->current_basic_block_number];
  memset(printed_bbs, 0, sizeof(printed_bbs));

  // Print all global declarations
  for (struct gkcc_ir_symbol_list *slist = ir_full->global_symbols;
       slist != NULL; slist = slist->next) {
    if (slist->symbol->ystring != NULL) {
      // This is a string
      char buf[(1 << 12) + 1];
      sprint_escaped_string(buf, slist->symbol->ystring->raw,
                            slist->symbol->ystring->length);
      printf("%s:\n", slist->symbol->symbol->symbol_name);
      printf("\t.string \"%s\"\n", buf);
      continue;
    }
    printf(".comm global:%s, %d, 4\n", slist->symbol->symbol->symbol_name,
           gkcc_type_sizeof(slist->symbol->symbol->symbol_type));
  }

  printf("\n");

  // Print all functions
  for (struct gkcc_ir_function_list *fn_list = ir_full->function_list;
       fn_list != NULL; fn_list = fn_list->next) {
    struct gkcc_ir_function *fn = fn_list->fn;
    printf("FN_%s:\n", fn->function_name);

    // Print all BBs
    gkcc_basic_block_print(printed_bbs, fn->entrance_basic_block);
  }
}

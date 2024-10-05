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

#ifndef GKCC_IR_FULL_H
#define GKCC_IR_FULL_H

#include "ir/ir_base.h"
#include "ir/quads.h"

// ====================================
// === struct gkcc_ir_function_list ===
// ====================================

struct gkcc_ir_function_list {
  struct gkcc_ir_function *fn;
  struct gkcc_ir_function_list *next;
};

// ===========================
// === struct gkcc_ir_full ===
// ===========================

struct gkcc_ir_full {
  struct gkcc_ir_function_list *function_list;
  struct gkcc_ir_symbol_list *global_symbols;
  struct gkcc_ir_generation_state *gen_state;
};

// =============================
// === FUNCTION DECLARATIONS ===
// =============================

struct gkcc_ir_full *gkcc_ir_full_new(void);

struct gkcc_ir_full *gkcc_ir_build_full(struct ast_node *node);

struct gkcc_ir_function_list *gkcc_ir_function_list_append(
    struct gkcc_ir_function_list *fn_list, struct gkcc_ir_function *fn);

struct gkcc_ir_function_list *gkcc_ir_function_list_new(
    struct gkcc_ir_function *fn);

void gkcc_ir_full_print(struct gkcc_ir_full *ir_full);

struct gkcc_ir_symbol_list *gkcc_ir_symbol_list_new(
    struct gkcc_ir_symbol *symbol);

struct gkcc_ir_symbol_list *gkcc_ir_symbol_list_append(
    struct gkcc_ir_symbol_list *list, struct gkcc_ir_symbol *symbol);

struct gkcc_ir_symbol *gkcc_ir_symbol_new(struct gkcc_symbol *gs,
                                          bool is_global);
#endif  // GKCC_IR_FULL_H

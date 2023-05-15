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

#ifndef GKCC_QUADS_H
#define GKCC_QUADS_H

#include <stdbool.h>

#include "ast/ast.h"
#include "ir/basic_block.h"
#include "misc/misc.h"

// =====================================
// === struct gkcc_ir_pseudoregister ===
// =====================================

struct gkcc_ir_pseudoregister {
  int register_num;
};

// =============================
// === struct gkcc_ir_symbol ===
// =============================

struct gkcc_ir_symbol {
  bool is_global;
  struct gkcc_symbol *symbol;
};

// ==================================
// === struct gkcc_ir_symbol_list ===
// ==================================

struct gkcc_ir_symbol_list {
  struct gkcc_ir_symbol *symbol;
  struct gkcc_ir_symbol_list *next;
};

// ====================================
// === struct gkcc_ir_quad_register ===
// ====================================

#define ENUM_GKCC_IR_QUAD_REGISTER_TYPE(GEN) \
  GEN(GKCC_IR_QUAD_REGISTER_PSEUDOREGISTER)  \
  GEN(GKCC_IR_QUAD_REGISTER_SYMBOL)          \
  GEN(GKCC_IR_QUAD_REGISTER_CONSTANT)        \
  GEN(GKCC_IR_QUAD_REGISTER_BASIC_BLOCK)

enum gkcc_ir_quad_register_type {
  ENUM_GKCC_IR_QUAD_REGISTER_TYPE(ENUM_VALUES)
};

static const char *const GKCC_IR_QUAD_REGISTER_TYPE_STRING[] = {
    ENUM_GKCC_IR_QUAD_REGISTER_TYPE(ENUM_STRINGS)};

#undef ENUM_IR_QUAD_REGISTER_TYPE

struct gkcc_ir_quad_register {
  enum gkcc_ir_quad_register_type register_type;
  struct gkcc_type *type;
  union {
    struct gkcc_ir_symbol symbol;
    struct ast_constant *constant;
    struct gkcc_ir_pseudoregister pseudoregister;
    struct gkcc_basic_block *basic_block;
  };
};

// ===========================
// === struct gkcc_ir_quad ===
// ===========================

#define ENUM_GKCC_IR_QUAD_INSTRUCTION(GEN)               \
  GEN(GKCC_IR_QUAD_INSTRUCTION_LEA)                      \
  GEN(GKCC_IR_QUAD_INSTRUCTION_LOAD)                     \
  GEN(GKCC_IR_QUAD_INSTRUCTION_STR)                      \
  GEN(GKCC_IR_QUAD_INSTRUCTION_ADD)                      \
  GEN(GKCC_IR_QUAD_INSTRUCTION_SUBTRACT)                 \
  GEN(GKCC_IR_QUAD_INSTRUCTION_DIVIDE)                   \
  GEN(GKCC_IR_QUAD_INSTRUCTION_MULTIPLY)                 \
  GEN(GKCC_IR_QUAD_INSTRUCTION_MOD)                      \
  GEN(GKCC_IR_QUAD_INSTRUCTION_BRANCH)                   \
  GEN(GKCC_IR_QUAD_INSTRUCTION_EQUALS)                   \
  GEN(GKCC_IR_QUAD_INSTRUCTION_BRANCH_IF_TRUE)           \
  GEN(GKCC_IR_QUAD_INSTRUCTION_BRANCH_IF_FALSE)          \
  GEN(GKCC_IR_QUAD_INSTRUCTION_MOVE)                     \
  GEN(GKCC_IR_QUAD_INSTRUCTION_GREATER_THAN)             \
  GEN(GKCC_IR_QUAD_INSTRUCTION_LESS_THAN)                \
  GEN(GKCC_IR_QUAD_INSTRUCTION_GREATER_THAN_OR_EQUAL_TO) \
  GEN(GKCC_IR_QUAD_INSTRUCTION_LESS_THAN_OR_EQUAL_TO)    \
  GEN(GKCC_IR_QUAD_INSTRUCTION_FUNCTION_CALL)            \
  GEN(GKCC_IR_QUAD_INSTRUCTION_FUNCION_ARG)              \
  GEN(GKCC_IR_QUAD_INSTRUCTION_LOGICAL_NOT)              \
  GEN(GKCC_IR_QUAD_INSTRUCTION_NEGATE_VALUE)             \
  GEN(GKCC_IR_QUAD_INSTRUCTION_POSTINC)                  \
  GEN(GKCC_IR_QUAD_INSTRUCTION_POSTDEC)                  \
  GEN(GKCC_IR_QUAD_INSTRUCTION_BITWISE_NOT)              \
  GEN(GKCC_IR_QUAD_INSTRUCTION_RETURN)

enum gkcc_ir_quad_instruction { ENUM_GKCC_IR_QUAD_INSTRUCTION(ENUM_VALUES) };

static const char *const GKCC_IR_QUAD_INSTRUCTION_STRING[] = {
    ENUM_GKCC_IR_QUAD_INSTRUCTION(ENUM_STRINGS)};

#undef ENUM_GKCC_IR_QUAD_INSTRUCTION

struct gkcc_ir_quad {
  enum gkcc_ir_quad_instruction instruction;
  struct gkcc_ir_quad_register *dest;
  struct gkcc_ir_quad_register *source1;
  struct gkcc_ir_quad_register *source2;
};

// ================================
// === struct gkcc_ir_quad_list ===
// ================================

struct gkcc_ir_quad_list {
  struct gkcc_ir_quad *quad;
  struct gkcc_ir_quad_list *next;
};

// ===============================
// === struct gkcc_ir_function ===
// ===============================

struct gkcc_ir_function {
  char *function_name;
  struct gkcc_basic_block *entrance_basic_block;
  int required_space_for_locals;
  int required_space_for_tmps;
};

// =======================================
// === struct gkcc_ir_generation_state ===
// =======================================

struct gkcc_ir_generation_state {
  int current_pseudoregister_number;
  int current_basic_block_number;
  struct gkcc_ir_function *current_function;
};

// =============================
// === FUNCTION DECLARATIONS ===
// =============================

struct gkcc_ir_quad_register *gkcc_ir_quad_register_new(
    enum gkcc_ir_quad_register_type register_type);

struct gkcc_ir_quad_register *gkcc_ir_quad_register_new_pseudoregister(
    struct gkcc_ir_generation_state *gen_state);

struct gkcc_ir_quad *gkcc_ir_quad_new(void);

struct gkcc_ir_quad_list *gkcc_ir_quad_list_new(void);

struct gkcc_ir_translation_result gkcc_ir_quad_generate_for_ast(
    struct gkcc_ir_generation_state *gen_state, struct ast_node *lnode);

struct gkcc_ir_quad_list *gkcc_ir_quad_list_append_list(
    struct gkcc_ir_quad_list *ql1, struct gkcc_ir_quad_list *ql2);

struct gkcc_ir_quad_list *gkcc_ir_quad_list_append(
    struct gkcc_ir_quad_list *ql1, struct gkcc_ir_quad *ta);

struct gkcc_ir_generation_state *gkcc_ir_generation_state_new(void);

void gkcc_ir_quad_print(struct gkcc_ir_quad_list *ql);

struct gkcc_ir_quad *gkcc_ir_quad_new_with_args(
    enum gkcc_ir_quad_instruction instruction,
    struct gkcc_ir_quad_register *dest, struct gkcc_ir_quad_register *source1,
    struct gkcc_ir_quad_register *source2);

struct gkcc_ir_quad_register *gkcc_ir_quad_register_new_basic_block(
    struct gkcc_basic_block *bb);

struct gkcc_ir_function *gkcc_ir_function_new(char *function_name);

struct gkcc_ir_quad_register *gkcc_ir_quad_register_new_int_constant(
    int constant);

struct gkcc_ir_translation_result gkcc_ir_quad_generate_declaration(
    struct gkcc_ir_generation_state *gen_state, struct ast_node *node);

#endif  // GKCC_QUADS_H

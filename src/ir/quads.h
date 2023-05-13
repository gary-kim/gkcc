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

#include "misc/misc.h"

// =====================================
// === struct gkcc_ir_pseudoregister ===
// =====================================

struct gkcc_ir_pseudoregister {
  int register_num;
};

// ====================================
// === struct gkcc_ir_quad_register ===
// ====================================

#define ENUM_GKCC_IR_QUAD_REGISTER_TYPE(GEN) \
  GEN(GKCC_IR_QUAD_REGISTER_PSEUDOREGISTER)  \
  GEN(GKCC_IR_QUAD_REGISTER_SYMBOL)

enum gkcc_ir_quad_register_type {
  ENUM_GKCC_IR_QUAD_REGISTER_TYPE(ENUM_VALUES)
};

static const char* const GKCC_IR_QUAD_REGISTER_TYPE_STRING[] = {
    ENUM_GKCC_IR_QUAD_REGISTER_TYPE(ENUM_STRINGS)};

#undef ENUM_IR_QUAD_REGISTER_TYPE

struct gkcc_ir_quad_register {
  enum gkcc_ir_quad_register_type register_type;
  union {
    struct gkcc_symbol* symbol;
    struct gkcc_ir_pseudoregister pseudoregister;
  };
};

// ===========================
// === struct gkcc_ir_quad ===
// ===========================

#define ENUM_GKCC_IR_QUAD_INSTRUCTION(GEN) GEN(GKCC_IR_QUAD_INSTRUCTION_LEA)

enum gkcc_ir_quad_instruction { ENUM_GKCC_IR_QUAD_INSTRUCTION(ENUM_VALUES) };

static const char* const GKCC_IR_QUAD_INSTRUCTION_STRING[] = {
    ENUM_GKCC_IR_QUAD_INSTRUCTION(ENUM_STRINGS)};

#undef ENUM_GKCC_IR_QUAD_INSTRUCTION

struct gkcc_ir_quad {
  enum gkcc_ir_quad_instruction instruction;
  struct gkcc_ir_quad_register dest;
  struct gkcc_ir_quad_register source1;
  struct gkcc_ir_quad_register source2;
};

// ================================
// === struct gkcc_ir_quad_list ===
// ================================

struct gkcc_ir_quad_list {
  struct gkcc_ir_quad *quad;
  struct gkcc_ir_quad_list *next;
};

#endif  // GKCC_QUADS_H

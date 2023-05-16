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

#ifndef GKCC_X86_INST_H
#define GKCC_X86_INST_H

#include <stdio.h>

#include "ir/quads.h"

void gkcc_tx86_translate_ir_quad_load_into_registers(FILE *out_file,
                                                     struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_load(FILE *out_file,
                                                  struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_return(FILE *out_file,
                                                    struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_add(FILE *out_file,
                                                 struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_subtract(
    FILE *out_file, struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_multiply(
    FILE *out_file, struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_divide(FILE *out_file,
                                                    struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_mod(FILE *out_file,
                                                 struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_save_register(FILE *out_file,
                                               struct gkcc_ir_quad *quad,
                                               char *from_register);

void gkcc_tx86_translate_ir_quad_instruction_function_arg(
    FILE *out_file, struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_function_call(
    FILE *out_file, struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_branch_if_true(
    FILE *out_file, struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_branch_if_false(
    FILE *out_file, struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_branch(FILE *out_file,
                                                    struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_equals(FILE *out_file,
                                                    struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_move(FILE *out_file,
                                                  struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_logical_not(
    FILE *out_file, struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_greater_than(
    FILE *out_file, struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_less_than(
    FILE *out_file, struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_greater_than_or_equal_to(
    FILE *out_file, struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_less_than_or_equal_to(
    FILE *out_file, struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_lea(FILE *out_file,
                                                 struct gkcc_ir_quad *quad);

void gkcc_tx86_translate_ir_quad_instruction_str(FILE *out_file,
                                                 struct gkcc_ir_quad *quad);

#endif  // GKCC_X86_INST_H

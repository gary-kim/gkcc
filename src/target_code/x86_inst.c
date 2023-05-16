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

#include "x86_inst.h"

#include "x86.h"

char buf1[(1 << 12) + 1];
char buf2[(1 << 12) + 1];
void gkcc_tx86_translate_ir_quad_load_into_registers(
    FILE *out_file, struct gkcc_ir_quad *quad) {
  if (quad->source1)
    fprintf(out_file, "\tmovl %s, %%eax\n",
            gkcc_tx86_translate_ir_quad_register(buf1, quad->source1));
  if (quad->source2)
    fprintf(out_file, "\tmovl %s, %%edx\n",
            gkcc_tx86_translate_ir_quad_register(buf1, quad->source2));
}

void gkcc_tx86_translate_ir_quad_save_register(FILE *out_file,
                                               struct gkcc_ir_quad *quad,
                                               char *from_register) {
  if (quad->dest)
    fprintf(out_file, "\tmovl %s, %s\n", from_register,
            gkcc_tx86_translate_ir_quad_register(buf1, quad->dest));
}

void gkcc_tx86_translate_ir_quad_instruction_load(FILE *out_file,
                                                  struct gkcc_ir_quad *quad) {
  gkcc_tx86_translate_ir_quad_load_into_registers(out_file, quad);
  fprintf(out_file, "\tmovl (%%eax), %%eax\n");
  gkcc_tx86_translate_ir_quad_save_register(out_file, quad, "%eax");
}
void gkcc_tx86_translate_ir_quad_instruction_return(FILE *out_file,
                                                    struct gkcc_ir_quad *quad) {
  gkcc_tx86_translate_ir_quad_load_into_registers(out_file, quad);

  fprintf(out_file, "\tleave\n");
  fprintf(out_file, "\tret\n");
}
void gkcc_tx86_translate_ir_quad_instruction_add(FILE *out_file,
                                                 struct gkcc_ir_quad *quad) {
  gkcc_tx86_translate_ir_quad_load_into_registers(out_file, quad);
  fprintf(out_file, "\taddl %%edx, %%eax\n");
  gkcc_tx86_translate_ir_quad_save_register(out_file, quad, "%eax");
}

void gkcc_tx86_translate_ir_quad_instruction_subtract(
    FILE *out_file, struct gkcc_ir_quad *quad) {
  gkcc_tx86_translate_ir_quad_load_into_registers(out_file, quad);
  fprintf(out_file, "\tsubl %%edx, %%eax\n");
  gkcc_tx86_translate_ir_quad_save_register(out_file, quad, "%eax");
}

void gkcc_tx86_translate_ir_quad_instruction_multiply(
    FILE *out_file, struct gkcc_ir_quad *quad) {
  gkcc_tx86_translate_ir_quad_load_into_registers(out_file, quad);
  fprintf(out_file, "\timull %%edx, %%eax\n");
  gkcc_tx86_translate_ir_quad_save_register(out_file, quad, "%eax");
}

void gkcc_tx86_translate_ir_quad_instruction_divide(FILE *out_file,
                                                    struct gkcc_ir_quad *quad) {
  fprintf(out_file, "\tmovl %s, %%ecx\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->source2));
  fprintf(out_file, "\tmovl %s, %%eax\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->source1));
  fprintf(out_file, "\tcltd\n");
  fprintf(out_file, "\tidivl %%ecx\n");
  gkcc_tx86_translate_ir_quad_save_register(out_file, quad, "%eax");
}

void gkcc_tx86_translate_ir_quad_instruction_mod(FILE *out_file,
                                                 struct gkcc_ir_quad *quad) {
  fprintf(out_file, "\tmovl %s, %%ecx\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->source2));
  fprintf(out_file, "\tmovl %s, %%eax\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->source1));
  fprintf(out_file, "\tcltd\n");
  fprintf(out_file, "\tidivl %%ecx\n");
  gkcc_tx86_translate_ir_quad_save_register(out_file, quad, "%edx");
}

void gkcc_tx86_translate_ir_quad_instruction_function_arg(
    FILE *out_file, struct gkcc_ir_quad *quad) {
  fprintf(out_file, "\tpushl %s\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->source1));
}

void gkcc_tx86_translate_ir_quad_instruction_function_call(
    FILE *out_file, struct gkcc_ir_quad *quad) {
  fprintf(out_file, "\tcall %s\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->source1));
  gkcc_tx86_translate_ir_quad_save_register(out_file, quad, "%eax");
}

void gkcc_tx86_translate_ir_quad_instruction_branch_if_true(
    FILE *out_file, struct gkcc_ir_quad *quad) {
  gkcc_tx86_translate_ir_quad_load_into_registers(out_file, quad);
  fprintf(out_file, "\tmovl %s, %%eax\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->source2));
  fprintf(out_file, "\tcmpl $0, %%eax\n");
  fprintf(out_file, "\tjne %s\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->source1));
}

void gkcc_tx86_translate_ir_quad_instruction_branch_if_false(
    FILE *out_file, struct gkcc_ir_quad *quad) {
  fprintf(out_file, "\tmovl %s, %%eax\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->source2));
  fprintf(out_file, "\tcmpl $0, %%eax\n");
  fprintf(out_file, "\tje %s\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->source1));
}

void gkcc_tx86_translate_ir_quad_instruction_branch(FILE *out_file,
                                                    struct gkcc_ir_quad *quad) {
  fprintf(out_file, "\tjmp %s\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->source1));
}

void gkcc_tx86_translate_ir_quad_instruction_equals(FILE *out_file,
                                                    struct gkcc_ir_quad *quad) {
  gkcc_tx86_translate_ir_quad_load_into_registers(out_file, quad);
  fprintf(out_file, "\tmovl $0, %s\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->dest));
  fprintf(out_file, "\tcmpl %%eax, %%edx\n");
  fprintf(out_file, "\tsete %s\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->dest));
}

void gkcc_tx86_translate_ir_quad_instruction_move(FILE *out_file,
                                                  struct gkcc_ir_quad *quad) {
  fprintf(out_file, "\tmovl %s, %%eax\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->source1));
  fprintf(out_file, "\tmovl %%eax, %s\n",
          gkcc_tx86_translate_ir_quad_register(buf2, quad->dest));
}

void gkcc_tx86_translate_ir_quad_instruction_logical_not(
    FILE *out_file, struct gkcc_ir_quad *quad) {
  gkcc_tx86_translate_ir_quad_load_into_registers(out_file, quad);
  fprintf(out_file, "\tmovl $0, %s\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->dest));
  fprintf(out_file, "\tcmpl $0, %%eax\n");
  fprintf(out_file, "\tsete %s\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->dest));
}

void gkcc_tx86_translate_ir_quad_instruction_greater_than(
    FILE *out_file, struct gkcc_ir_quad *quad) {
  gkcc_tx86_translate_ir_quad_load_into_registers(out_file, quad);
  fprintf(out_file, "\tmovl $0, %s\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->dest));
  fprintf(out_file, "\tcmpl %%edx, %%eax\n");
  fprintf(out_file, "\tsetg %s\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->dest));
}

void gkcc_tx86_translate_ir_quad_instruction_less_than(
    FILE *out_file, struct gkcc_ir_quad *quad) {
  gkcc_tx86_translate_ir_quad_load_into_registers(out_file, quad);
  fprintf(out_file, "\tmovl $0, %s\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->dest));
  fprintf(out_file, "\tcmpl %%edx, %%eax\n");
  fprintf(out_file, "\tsetl %s\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->dest));
}
void gkcc_tx86_translate_ir_quad_instruction_greater_than_or_equal_to(
    FILE *out_file, struct gkcc_ir_quad *quad) {
  gkcc_tx86_translate_ir_quad_load_into_registers(out_file, quad);
  fprintf(out_file, "\tmovl $0, %s\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->dest));
  fprintf(out_file, "\tcmpl %%edx, %%eax\n");
  fprintf(out_file, "\tsetge %s\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->dest));
}
void gkcc_tx86_translate_ir_quad_instruction_less_than_or_equal_to(
    FILE *out_file, struct gkcc_ir_quad *quad) {
  gkcc_tx86_translate_ir_quad_load_into_registers(out_file, quad);
  fprintf(out_file, "\tmovl $0, %s\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->dest));
  fprintf(out_file, "\tcmpl %%edx, %%eax\n");
  fprintf(out_file, "\tsetle %s\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->dest));
}

void gkcc_tx86_translate_ir_quad_instruction_lea(FILE *out_file,
                                                 struct gkcc_ir_quad *quad) {
  fprintf(out_file, "\tleal %s, %%eax\n",
          gkcc_tx86_translate_ir_quad_register(buf1, quad->source1));
  gkcc_tx86_translate_ir_quad_save_register(out_file, quad, "%eax");
}

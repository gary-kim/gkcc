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

#include "x86.h"

#include <memory.h>
#include <stdio.h>

#include "ir/quads.h"
#include "target_code/x86_inst.h"

char *gkcc_tx86_translate_ir_quad_register(char *buf,
                                           struct gkcc_ir_quad_register *qr) {
  switch (qr->register_type) {
    case GKCC_IR_QUAD_REGISTER_PSEUDOREGISTER:
      sprintf(buf, "-%d(%%ebp)", qr->pseudoregister.offset);
      break;
    case GKCC_IR_QUAD_REGISTER_SYMBOL:
      if (qr->symbol.is_global) {
        if (qr->symbol.ystring == NULL) {
          sprintf(buf, "%s", qr->symbol.symbol->symbol_name);
        } else {
          sprintf(buf, "$%s", qr->symbol.symbol->symbol_name);
        }
      } else {
        sprintf(buf, "-%d(%%ebp)", qr->symbol.symbol->offset);
      }
      break;
    case GKCC_IR_QUAD_REGISTER_CONSTANT:
      buf[0] = '$';
      gkcc_ir_quad_register_constant_string(&buf[1], qr);
      break;
    case GKCC_IR_QUAD_REGISTER_BASIC_BLOCK:
      sprintf(buf, "%s", qr->basic_block->bb_name);
      break;
  }
  return buf;
}

void gkcc_tx86_translate_ir_quad(FILE *out_file, struct gkcc_ir_quad *quad) {
  char buf1[(1 << 12) + 1];
  char buf2[(1 << 12) + 1];
  static int current_function_push_val = 0;
  switch (quad->instruction) {
    case GKCC_IR_QUAD_INSTRUCTION_LEA:
      // TODO: Cannot be doing an address to address move
      gkcc_tx86_translate_ir_quad_instruction_lea(out_file, quad);
      break;
    case GKCC_IR_QUAD_INSTRUCTION_LOAD:
      gkcc_tx86_translate_ir_quad_instruction_load(out_file, quad);
      break;
    case GKCC_IR_QUAD_INSTRUCTION_STR:
      gkcc_tx86_translate_ir_quad_instruction_str(out_file, quad);
      break;
    case GKCC_IR_QUAD_INSTRUCTION_ADD:
      gkcc_tx86_translate_ir_quad_instruction_add(out_file, quad);
      break;
    case GKCC_IR_QUAD_INSTRUCTION_SUBTRACT:
      gkcc_tx86_translate_ir_quad_instruction_subtract(out_file, quad);
      break;
    case GKCC_IR_QUAD_INSTRUCTION_DIVIDE:
      gkcc_tx86_translate_ir_quad_instruction_divide(out_file, quad);
      break;
    case GKCC_IR_QUAD_INSTRUCTION_MULTIPLY:
      gkcc_tx86_translate_ir_quad_instruction_multiply(out_file, quad);
      break;
    case GKCC_IR_QUAD_INSTRUCTION_MOD:
      gkcc_tx86_translate_ir_quad_instruction_mod(out_file, quad);
      break;
    case GKCC_IR_QUAD_INSTRUCTION_BRANCH:
      gkcc_tx86_translate_ir_quad_instruction_branch(out_file, quad);
      break;
    case GKCC_IR_QUAD_INSTRUCTION_EQUALS:
      gkcc_tx86_translate_ir_quad_instruction_equals(out_file, quad);
      break;
    case GKCC_IR_QUAD_INSTRUCTION_BRANCH_IF_TRUE:
      gkcc_tx86_translate_ir_quad_instruction_branch_if_true(out_file, quad);
      break;
    case GKCC_IR_QUAD_INSTRUCTION_BRANCH_IF_FALSE:
      gkcc_tx86_translate_ir_quad_instruction_branch_if_false(out_file, quad);
      break;
    case GKCC_IR_QUAD_INSTRUCTION_MOVE:
      gkcc_tx86_translate_ir_quad_instruction_move(out_file, quad);
      break;
    case GKCC_IR_QUAD_INSTRUCTION_GREATER_THAN:
      gkcc_tx86_translate_ir_quad_instruction_greater_than(out_file, quad);
      break;
    case GKCC_IR_QUAD_INSTRUCTION_LESS_THAN:
      gkcc_tx86_translate_ir_quad_instruction_less_than(out_file, quad);
      break;
    case GKCC_IR_QUAD_INSTRUCTION_GREATER_THAN_OR_EQUAL_TO:
      gkcc_tx86_translate_ir_quad_instruction_greater_than_or_equal_to(out_file,
                                                                       quad);
      break;
    case GKCC_IR_QUAD_INSTRUCTION_LESS_THAN_OR_EQUAL_TO:
      gkcc_tx86_translate_ir_quad_instruction_less_than_or_equal_to(out_file,
                                                                    quad);
      break;
    case GKCC_IR_QUAD_INSTRUCTION_FUNCTION_CALL:
      gkcc_tx86_translate_ir_quad_instruction_function_call(out_file, quad);
      fprintf(out_file, "\taddl $%lu, %%esp\n",
              current_function_push_val * sizeof(int));
      current_function_push_val = 0;
      break;
    case GKCC_IR_QUAD_INSTRUCTION_FUNCION_ARG:
      gkcc_tx86_translate_ir_quad_instruction_function_arg(out_file, quad);
      current_function_push_val++;
      break;
    case GKCC_IR_QUAD_INSTRUCTION_LOGICAL_NOT:
      gkcc_tx86_translate_ir_quad_instruction_logical_not(out_file, quad);
      break;
    case GKCC_IR_QUAD_INSTRUCTION_NEGATE_VALUE:
      break;
    case GKCC_IR_QUAD_INSTRUCTION_POSTINC:
      break;
    case GKCC_IR_QUAD_INSTRUCTION_POSTDEC:
      break;
    case GKCC_IR_QUAD_INSTRUCTION_BITWISE_NOT:
      break;
    case GKCC_IR_QUAD_INSTRUCTION_RETURN:
      gkcc_tx86_translate_ir_quad_instruction_return(out_file, quad);
      break;
  }
}

void gkcc_tx86_function_preamble(FILE *out_file, struct gkcc_ir_function *fn) {
  int total_stack_space = fn->required_space_for_locals;
  total_stack_space += 16 - (total_stack_space % 16);
  fprintf(out_file, "\tpushl %%ebp\n");
  fprintf(out_file, "\tmovl %%esp, %%ebp\n");
  fprintf(out_file, "\tsubl $%d, %%esp\n", total_stack_space);
  return;
}

bool gkcc_internal_tx86_is_load_lea_str(struct gkcc_ir_quad_list *ql) {
  if (ql == NULL || ql->next == NULL || ql->next->next == NULL) return false;
  if (ql->quad->instruction == GKCC_IR_QUAD_INSTRUCTION_LOAD &&
      ql->next->quad->instruction == GKCC_IR_QUAD_INSTRUCTION_LEA &&
      ql->next->next->quad->instruction == GKCC_IR_QUAD_INSTRUCTION_STR) {
    // It is a shorten-able instruction
    ql->next->next->quad->dest = ql->quad->source1;

    return true;
  }
  return false;
}

void gkcc_tx86_print_bb(FILE *out_file, bool *printed,
                        struct gkcc_basic_block *bb) {
  if (bb == NULL) return;
  if (printed[bb->bb_number]) return;

  printed[bb->bb_number] = true;
  fprintf(out_file, "%s:\n", bb->bb_name);
  for (struct gkcc_ir_quad_list *ql = bb->quads_in_bb; ql != NULL;
       ql = ql->next) {
    // If there is a (load, lea, str), collapse it into a single store
    if (gkcc_internal_tx86_is_load_lea_str(ql)) {
      ql = ql->next;
      continue;
    }
    gkcc_tx86_translate_ir_quad(out_file, ql->quad);
  }

  gkcc_tx86_print_bb(out_file, printed, bb->true_branch);
  if (bb->true_branch != bb->false_branch)
    gkcc_tx86_print_bb(out_file, printed, bb->false_branch);
}

void gkcc_tx86_generate_ir_full(FILE *out_file, struct gkcc_ir_full *ir_full) {
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
      fprintf(out_file, "\t.section .rodata\n");
      fprintf(out_file, "%s:\n", slist->symbol->symbol->symbol_name);
      fprintf(out_file, "\t.string \"%s\"\n", buf);
      fprintf(out_file, "\t.text\n");
      continue;
    }
    fprintf(out_file, ".comm %s, %d, 4\n", slist->symbol->symbol->symbol_name,
            gkcc_type_sizeof(slist->symbol->symbol->symbol_type));
  }

  fprintf(out_file, "\n");

  for (struct gkcc_ir_function_list *fn_list = ir_full->function_list;
       fn_list != NULL; fn_list = fn_list->next) {
    struct gkcc_ir_function *fn = fn_list->fn;
    fprintf(out_file, ".globl %s\n", fn->function_name);
    fprintf(out_file, "%s:\n", fn->function_name);
    gkcc_tx86_function_preamble(out_file, fn);

    // Print all BBs
    gkcc_tx86_print_bb(out_file, printed_bbs, fn->entrance_basic_block);
  }
}

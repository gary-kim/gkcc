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

#include "ir/translators.h"

#include "ast/ast.h"

#define ADD_INST(ARG) \
  tr.ir_quad_list = gkcc_ir_quad_list_append(tr.ir_quad_list, ARG)

struct gkcc_ir_translation_result gkcc_ir_translate_ast_ident(
    struct gkcc_ir_generation_state* gen_state, struct ast_ident* ident) {
  struct gkcc_ir_quad_register* ir_register =
      gkcc_ir_quad_register_new(GKCC_IR_QUAD_REGISTER_SYMBOL);
  ir_register->symbol.symbol = ident->symbol_table_entry;
  ir_register->symbol.is_global =
      ident->symbol_table_entry->symbol_table_set->scope == GKCC_SCOPE_GLOBAL;
  struct gkcc_ir_translation_result translation_result = {
      .result = ir_register,
      .ir_quad_list = NULL,
  };
  return translation_result;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_constant(
    struct gkcc_ir_generation_state* gen_state, struct ast_constant* constant) {
  struct gkcc_ir_quad_register* ir_register =
      gkcc_ir_quad_register_new(GKCC_IR_QUAD_REGISTER_CONSTANT);
  ir_register->constant = constant;
  struct gkcc_ir_translation_result translation_result = {
      .result = ir_register,
      .ir_quad_list = NULL,
  };
  return translation_result;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_add(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right) {
  tr.result = gkcc_ir_quad_register_new_pseudoregister(gen_state);
  ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_ADD, tr.result,
                                      translation_result_left.result,
                                      translation_result_right.result));
  return tr;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_subtract(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right) {
  tr.result = gkcc_ir_quad_register_new_pseudoregister(gen_state);
  ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_SUBTRACT,
                                      tr.result, translation_result_left.result,
                                      translation_result_right.result));
  return tr;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_multiply(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right) {
  tr.result = gkcc_ir_quad_register_new_pseudoregister(gen_state);
  ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_MULTIPLY,
                                      tr.result, translation_result_left.result,
                                      translation_result_right.result));
  return tr;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_divide(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right) {
  tr.result = gkcc_ir_quad_register_new_pseudoregister(gen_state);
  ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_DIVIDE,
                                      tr.result, translation_result_left.result,
                                      translation_result_right.result));
  return tr;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_mod(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right) {
  tr.result = gkcc_ir_quad_register_new_pseudoregister(gen_state);
  ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_MOD, tr.result,
                                      translation_result_left.result,
                                      translation_result_right.result));
  return tr;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_equals(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right) {
  tr.result = gkcc_ir_quad_register_new_pseudoregister(gen_state);
  ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_EQUALS,
                                      tr.result, translation_result_left.result,
                                      translation_result_right.result));
  return tr;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop(
    struct gkcc_ir_generation_state* gen_state, struct ast_node* node) {
  struct gkcc_ir_translation_result translation_result_left =
      gkcc_ir_quad_generate_for_ast(gen_state, node->binop.left);
  struct gkcc_ir_translation_result translation_result_right =
      gkcc_ir_quad_generate_for_ast(gen_state, node->binop.right);
  struct gkcc_ir_translation_result final_result = {
      .ir_quad_list = translation_result_left.ir_quad_list,
      .result = NULL,
  };
  final_result.ir_quad_list = gkcc_ir_quad_list_append_list(
      final_result.ir_quad_list, translation_result_right.ir_quad_list);
  switch (node->binop.type) {
    case AST_BINOP_ADD:
      return gkcc_ir_translate_ast_node_binop_add(
          gen_state, &node->binop, final_result, translation_result_left,
          translation_result_right);
    case AST_BINOP_SUBTRACT:
      return gkcc_ir_translate_ast_node_binop_subtract(
          gen_state, &node->binop, final_result, translation_result_left,
          translation_result_right);
    case AST_BINOP_DIVIDE:
      return gkcc_ir_translate_ast_node_binop_divide(
          gen_state, &node->binop, final_result, translation_result_left,
          translation_result_right);
    case AST_BINOP_MULTIPLY:
      return gkcc_ir_translate_ast_node_binop_multiply(
          gen_state, &node->binop, final_result, translation_result_left,
          translation_result_right);
    case AST_BINOP_MOD:
      return gkcc_ir_translate_ast_node_binop_mod(
          gen_state, &node->binop, final_result, translation_result_left,
          translation_result_right);
    case AST_BINOP_ASSIGN:
      break;
    case AST_BINOP_ASSIGN_MULTIPLY:
      break;
    case AST_BINOP_ASSIGN_DIVIDE:
      break;
    case AST_BINOP_ASSIGN_MOD:
      break;
    case AST_BINOP_ASSIGN_ADD:
      break;
    case AST_BINOP_ASSIGN_SUBTRACT:
      break;
    case AST_BINOP_ASSIGN_BITWISE_SHL:
      break;
    case AST_BINOP_ASSIGN_BITWISE_SHR:
      break;
    case AST_BINOP_ASSIGN_BITWISE_AND:
      break;
    case AST_BINOP_ASSIGN_BITWISE_XOR:
      break;
    case AST_BINOP_ASSIGN_BITWISE_OR:
      break;
    case AST_BINOP_SHL:
      break;
    case AST_BINOP_SHR:
      break;
    case AST_BINOP_EQUALS:
      return gkcc_ir_translate_ast_node_binop_equals(
          gen_state, &node->binop, final_result, translation_result_left,
          translation_result_right);
    case AST_BINOP_LT:
      break;
    case AST_BINOP_GT:
      break;
    case AST_BINOP_LTEQ:
      break;
    case AST_BINOP_GTEQ:
      break;
    case AST_BINOP_BITWISE_AND:
      break;
    case AST_BINOP_BITWISE_XOR:
      break;
    case AST_BINOP_BITWISE_OR:
      break;
    case AST_BINOP_LOGICAL_AND:
      break;
    case AST_BINOP_LOGICAL_OR:
      break;
    case AST_BINOP_MEMBER_ACCESS:
      break;
    case AST_BINOP_CAST:
      break;
  }
  gkcc_error_fatal(GKCC_ERROR_NOT_YET_IMPLEMENTED,
                   "reached end of gkcc_ir_translate_ast_binop() without "
                   "running into implemented function");
  return final_result;
}

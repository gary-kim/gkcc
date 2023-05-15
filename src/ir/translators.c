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
  ir_register->type = ident->symbol_table_entry->symbol_type;
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
  struct gkcc_ir_quad_register* lresult = translation_result_left.result;
  struct gkcc_ir_quad_register* rresult = translation_result_right.result;
  if (!gkcc_is_gkcc_type_scalar(translation_result_left.result->type) &&
      !gkcc_is_gkcc_type_scalar(translation_result_right.result->type)) {
    gkcc_error_fatal(GKCC_ERROR_INVALID_CODE,
                     "Found attempt to add a pointer with a pointer. This is "
                     "NOT ALLOWED!!!");
  }

  if (!gkcc_is_gkcc_type_scalar(translation_result_left.result->type)) {
    struct gkcc_ir_translation_result intermediate = {
        .result = gkcc_ir_quad_register_new_pseudoregister(gen_state),
        .ir_quad_list = NULL,
    };
    intermediate.result->type = translation_result_left.result->type->of;
    rresult = intermediate.result;
    struct gkcc_ir_quad_register* size_register =
        gkcc_ir_quad_register_new_int_constant(
            gkcc_type_sizeof(intermediate.result->type));
    ADD_INST(gkcc_ir_quad_new_with_args(
        GKCC_IR_QUAD_INSTRUCTION_MULTIPLY, intermediate.result,
        translation_result_right.result, size_register));
    tr.result->type = translation_result_left.result->type;
  }
  if (!gkcc_is_gkcc_type_scalar(translation_result_right.result->type)) {
    struct gkcc_ir_translation_result intermediate = {
        .result = gkcc_ir_quad_register_new_pseudoregister(gen_state),
        .ir_quad_list = NULL,
    };
    intermediate.result->type = translation_result_right.result->type->of;
    lresult = intermediate.result;
    struct gkcc_ir_quad_register* size_register =
        gkcc_ir_quad_register_new_int_constant(
            gkcc_type_sizeof(intermediate.result->type));
    ADD_INST(gkcc_ir_quad_new_with_args(
        GKCC_IR_QUAD_INSTRUCTION_MULTIPLY, intermediate.result,
        translation_result_left.result, size_register));
    tr.result->type = translation_result_right.result->type;
  }
  ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_ADD, tr.result,
                                      lresult, rresult));
  return tr;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_subtract(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right) {
  tr.result = gkcc_ir_quad_register_new_pseudoregister(gen_state);
  struct gkcc_ir_quad_register* lresult = translation_result_left.result;
  struct gkcc_ir_quad_register* rresult = translation_result_right.result;
  if (!gkcc_is_gkcc_type_scalar(translation_result_left.result->type) &&
      !gkcc_is_gkcc_type_scalar(translation_result_right.result->type)) {
    // TODO: Add check to make sure both sides are the same type
  } else if (!gkcc_is_gkcc_type_scalar(translation_result_left.result->type)) {
    struct gkcc_ir_translation_result intermediate = {
        .result = gkcc_ir_quad_register_new_pseudoregister(gen_state),
        .ir_quad_list = NULL,
    };
    intermediate.result->type = translation_result_left.result->type->of;
    rresult = intermediate.result;
    struct gkcc_ir_quad_register* size_register =
        gkcc_ir_quad_register_new_int_constant(
            gkcc_type_sizeof(intermediate.result->type));
    ADD_INST(gkcc_ir_quad_new_with_args(
        GKCC_IR_QUAD_INSTRUCTION_MULTIPLY, intermediate.result,
        translation_result_right.result, size_register));
    tr.result->type = translation_result_left.result->type;
  } else if (!gkcc_is_gkcc_type_scalar(translation_result_right.result->type)) {
    struct gkcc_ir_translation_result intermediate = {
        .result = gkcc_ir_quad_register_new_pseudoregister(gen_state),
        .ir_quad_list = NULL,
    };
    intermediate.result->type = translation_result_right.result->type->of;
    lresult = intermediate.result;
    struct gkcc_ir_quad_register* size_register =
        gkcc_ir_quad_register_new_int_constant(
            gkcc_type_sizeof(intermediate.result->type));
    ADD_INST(gkcc_ir_quad_new_with_args(
        GKCC_IR_QUAD_INSTRUCTION_MULTIPLY, intermediate.result,
        translation_result_left.result, size_register));
    tr.result->type = translation_result_right.result->type;
  }
  ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_SUBTRACT,
                                      tr.result, lresult, rresult));

  if (!gkcc_is_gkcc_type_scalar(translation_result_left.result->type) &&
      !gkcc_is_gkcc_type_scalar(translation_result_right.result->type)) {
    struct gkcc_ir_quad_register* old_result = tr.result;
    tr.result = gkcc_ir_quad_register_new_pseudoregister(gen_state);
    struct gkcc_ir_quad_register* size_register =
        gkcc_ir_quad_register_new_int_constant(
            gkcc_type_sizeof(translation_result_left.result->type->of));
    ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_DIVIDE,
                                        tr.result, old_result, size_register));
    tr.result->type = translation_result_left.result->type;
  }
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

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_gt(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right) {
  tr.result = gkcc_ir_quad_register_new_pseudoregister(gen_state);
  ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_GREATER_THAN,
                                      tr.result, translation_result_left.result,
                                      translation_result_right.result));
  return tr;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_lt(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right) {
  tr.result = gkcc_ir_quad_register_new_pseudoregister(gen_state);
  ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_LESS_THAN,
                                      tr.result, translation_result_left.result,
                                      translation_result_right.result));
  return tr;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_gteq(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right) {
  tr.result = gkcc_ir_quad_register_new_pseudoregister(gen_state);
  ADD_INST(gkcc_ir_quad_new_with_args(
      GKCC_IR_QUAD_INSTRUCTION_GREATER_THAN_OR_EQUAL_TO, tr.result,
      translation_result_left.result, translation_result_right.result));
  return tr;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_lteq(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right) {
  tr.result = gkcc_ir_quad_register_new_pseudoregister(gen_state);
  ADD_INST(gkcc_ir_quad_new_with_args(
      GKCC_IR_QUAD_INSTRUCTION_LESS_THAN_OR_EQUAL_TO, tr.result,
      translation_result_left.result, translation_result_right.result));
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

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_assign(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right) {
  ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_MOVE,
                                      translation_result_left.result,
                                      translation_result_right.result, NULL));
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
      return gkcc_ir_translate_ast_node_binop_assign(
          gen_state, &node->binop, final_result, translation_result_left,
          translation_result_right);
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
      return gkcc_ir_translate_ast_node_binop_lt(
          gen_state, &node->binop, final_result, translation_result_left,
          translation_result_right);
    case AST_BINOP_GT:
      return gkcc_ir_translate_ast_node_binop_gt(
          gen_state, &node->binop, final_result, translation_result_left,
          translation_result_right);
    case AST_BINOP_LTEQ:
      return gkcc_ir_translate_ast_node_binop_lteq(
          gen_state, &node->binop, final_result, translation_result_left,
          translation_result_right);
    case AST_BINOP_GTEQ:
      return gkcc_ir_translate_ast_node_binop_gteq(
          gen_state, &node->binop, final_result, translation_result_left,
          translation_result_right);
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
    case AST_BINOP_CAST:
      break;
  }
  gkcc_error_fatal(GKCC_ERROR_NOT_YET_IMPLEMENTED,
                   "reached end of gkcc_ir_translate_ast_binop() without "
                   "running into implemented function");
  return final_result;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_return(
    struct gkcc_ir_generation_state* gen_state,
    struct ast_function_return* fn_return) {
  struct gkcc_ir_translation_result return_value = {};
  if (fn_return->to_return != NULL) {
    return_value =
        gkcc_ir_quad_generate_for_ast(gen_state, fn_return->to_return);
  }

  struct gkcc_ir_translation_result tr = {
      .result = NULL,
      .ir_quad_list = NULL,
  };

  ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_RETURN, NULL,
                                      return_value.result, NULL));

  return tr;
}

struct gkcc_ir_translation_result
gkcc_internal_ir_translate_ast_function_call_args(
    struct gkcc_ir_generation_state* gen_state, struct ast_node* largs) {
  struct gkcc_ir_translation_result translation_result = {
      .result = gkcc_ir_quad_register_new_pseudoregister(gen_state),
      .ir_quad_list = NULL,
  };
  if (largs == NULL) {
    return translation_result;
  }
  if (largs->list.next != NULL) {
    struct gkcc_ir_translation_result rtr =
        gkcc_internal_ir_translate_ast_function_call_args(gen_state,
                                                          largs->list.next);
    translation_result.ir_quad_list = gkcc_ir_quad_list_append_list(
        translation_result.ir_quad_list, rtr.ir_quad_list);
  }

  struct ast_node* tnode = largs->list.node;
  struct gkcc_ir_translation_result this_op =
      gkcc_ir_quad_generate_for_ast(gen_state, tnode);
  translation_result.ir_quad_list = gkcc_ir_quad_list_append_list(
      translation_result.ir_quad_list, this_op.ir_quad_list);

  struct gkcc_ir_quad* quad = gkcc_ir_quad_new_with_args(
      GKCC_IR_QUAD_INSTRUCTION_FUNCION_ARG, NULL, this_op.result, NULL);
  translation_result.ir_quad_list =
      gkcc_ir_quad_list_append(translation_result.ir_quad_list, quad);
  return translation_result;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_function_call(
    struct gkcc_ir_generation_state* gen_state,
    struct ast_function_call* node) {
  struct gkcc_ir_translation_result result = {
      .result = gkcc_ir_quad_register_new_pseudoregister(gen_state),
      .ir_quad_list = NULL,
  };

  struct gkcc_ir_translation_result fn_translation =
      gkcc_ir_translate_ast_ident(gen_state, &node->name->ident);
  result.ir_quad_list = gkcc_ir_quad_list_append_list(
      result.ir_quad_list, fn_translation.ir_quad_list);

  struct gkcc_ir_translation_result args_translation =
      gkcc_internal_ir_translate_ast_function_call_args(gen_state,
                                                        node->parameters);
  result.ir_quad_list = gkcc_ir_quad_list_append_list(
      result.ir_quad_list, args_translation.ir_quad_list);

  result.ir_quad_list = gkcc_ir_quad_list_append(
      result.ir_quad_list,
      gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_FUNCTION_CALL, NULL,
                                 fn_translation.result, NULL));

  return result;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_unary_sizeof(
    struct gkcc_ir_generation_state* gen_state,
    struct gkcc_ir_translation_result prev_result,
    struct gkcc_ir_translation_result tr, struct ast_unary* unary) {
  tr.result = gkcc_ir_quad_register_new_int_constant(
      gkcc_type_sizeof(prev_result.result->type));
  return tr;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_unary_addressof(
    struct gkcc_ir_generation_state* gen_state,
    struct gkcc_ir_translation_result prev_result,
    struct gkcc_ir_translation_result tr, struct ast_unary* unary) {
  ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_LEA, tr.result,
                                      prev_result.result, NULL));
  tr.result->type = gkcc_type_new(GKCC_TYPE_PTR);
  tr.result->type->of = prev_result.result->type->of;
  return tr;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_unary_dereference(
    struct gkcc_ir_generation_state* gen_state,
    struct gkcc_ir_translation_result prev_result,
    struct gkcc_ir_translation_result tr, struct ast_unary* unary) {
  // Special handling for multidimensional arrays
  if (prev_result.result->type->type == GKCC_TYPE_ARRAY &&
      prev_result.result->type->of->type == GKCC_TYPE_ARRAY) {
    ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_MOVE,
                                        tr.result, prev_result.result, NULL));
  } else {
    ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_LOAD,
                                        tr.result, prev_result.result, NULL));
  }
  tr.result->type = prev_result.result->type->of;
  return tr;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_unary_not(
    struct gkcc_ir_generation_state* gen_state,
    struct gkcc_ir_translation_result prev_result,
    struct gkcc_ir_translation_result tr, struct ast_unary* unary) {
  ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_LOGICAL_NOT,
                                      tr.result, prev_result.result, NULL));
  return tr;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_unary_negative(
    struct gkcc_ir_generation_state* gen_state,
    struct gkcc_ir_translation_result prev_result,
    struct gkcc_ir_translation_result tr, struct ast_unary* unary) {
  ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_NEGATE_VALUE,
                                      tr.result, prev_result.result, NULL));
  return tr;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_unary_postinc(
    struct gkcc_ir_generation_state* gen_state,
    struct gkcc_ir_translation_result prev_result,
    struct gkcc_ir_translation_result tr, struct ast_unary* unary) {
  ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_POSTINC,
                                      tr.result, prev_result.result, NULL));
  return tr;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_unary_postdec(
    struct gkcc_ir_generation_state* gen_state,
    struct gkcc_ir_translation_result prev_result,
    struct gkcc_ir_translation_result tr, struct ast_unary* unary) {
  ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_POSTDEC,
                                      tr.result, prev_result.result, NULL));
  return tr;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_unary_bitwise_not(
    struct gkcc_ir_generation_state* gen_state,
    struct gkcc_ir_translation_result prev_result,
    struct gkcc_ir_translation_result tr, struct ast_unary* unary) {
  ADD_INST(gkcc_ir_quad_new_with_args(GKCC_IR_QUAD_INSTRUCTION_BITWISE_NOT,
                                      tr.result, prev_result.result, NULL));
  return tr;
}

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_unary(
    struct gkcc_ir_generation_state* gen_state, struct ast_unary* unary) {
  struct gkcc_ir_translation_result prev_result =
      gkcc_ir_quad_generate_for_ast(gen_state, unary->of);
  struct gkcc_ir_translation_result new_result = {
      .result = gkcc_ir_quad_register_new_pseudoregister(gen_state),
      .ir_quad_list =
          gkcc_ir_quad_list_append_list(NULL, prev_result.ir_quad_list),
  };
  new_result.result->type = prev_result.result->type;
  switch (unary->type) {
    case AST_UNARY_SIZEOF:
      return gkcc_ir_translate_ast_unary_sizeof(gen_state, prev_result,
                                                new_result, unary);
    case AST_UNARY_ADDRESSOF:
      return gkcc_ir_translate_ast_unary_addressof(gen_state, prev_result,
                                                   new_result, unary);
    case AST_UNARY_DEREFERENCE:
      return gkcc_ir_translate_ast_unary_dereference(gen_state, prev_result,
                                                     new_result, unary);
    case AST_UNARY_NEGATIVE:
      return gkcc_ir_translate_ast_unary_negative(gen_state, prev_result,
                                                  new_result, unary);
    case AST_UNARY_BITWISE_NOT:
      return gkcc_ir_translate_ast_unary_bitwise_not(gen_state, prev_result,
                                                     new_result, unary);
    case AST_UNARY_LOGICAL_NOT:
      return gkcc_ir_translate_ast_unary_not(gen_state, prev_result, new_result,
                                             unary);
    case AST_UNARY_POSTINC:
      return gkcc_ir_translate_ast_unary_postinc(gen_state, prev_result,
                                                 new_result, unary);
      break;
    case AST_UNARY_POSTDEC:
      return gkcc_ir_translate_ast_unary_postdec(gen_state, prev_result,
                                                 new_result, unary);
  }
}

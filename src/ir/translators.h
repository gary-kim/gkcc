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

#ifndef GKCC_TRANSLATORS_H
#define GKCC_TRANSLATORS_H

#include "ir/quads.h"

struct gkcc_ir_translation_result {
  struct gkcc_ir_quad_list* ir_quad_list;
  struct gkcc_ir_quad_register* result;
};

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop(
    struct gkcc_ir_generation_state* gen_state, struct ast_node* node);

struct gkcc_ir_quad_list* gkcc_ir_quad_list_append_list(
    struct gkcc_ir_quad_list* ql1, struct gkcc_ir_quad_list* ql2);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_constant(
    struct gkcc_ir_generation_state* gen_state, struct ast_constant* constant);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_ident(
    struct gkcc_ir_generation_state* gen_state, struct ast_ident* ident);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_add(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_subtract(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_multiply(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_divide(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_mod(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_equals(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_assign(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_gt(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_lt(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_gteq(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_lteq(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_function_call(
    struct gkcc_ir_generation_state* gen_state, struct ast_function_call* node);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_unary_sizeof(
    struct gkcc_ir_generation_state* gen_state,
    struct gkcc_ir_translation_result prev_result,
    struct gkcc_ir_translation_result tr, struct ast_unary* unary);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_unary(
    struct gkcc_ir_generation_state* gen_state, struct ast_unary* unary);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_unary_addressof(
    struct gkcc_ir_generation_state* gen_state,
    struct gkcc_ir_translation_result prev_result,
    struct gkcc_ir_translation_result tr, struct ast_unary* unary);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_unary_dereference(
    struct gkcc_ir_generation_state* gen_state,
    struct gkcc_ir_translation_result prev_result,
    struct gkcc_ir_translation_result tr, struct ast_unary* unary);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_unary_not(
    struct gkcc_ir_generation_state* gen_state,
    struct gkcc_ir_translation_result prev_result,
    struct gkcc_ir_translation_result tr, struct ast_unary* unary);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_unary_negative(
    struct gkcc_ir_generation_state* gen_state,
    struct gkcc_ir_translation_result prev_result,
    struct gkcc_ir_translation_result tr, struct ast_unary* unary);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_unary_postinc(
    struct gkcc_ir_generation_state* gen_state,
    struct gkcc_ir_translation_result prev_result,
    struct gkcc_ir_translation_result tr, struct ast_unary* unary);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_unary_postdec(
    struct gkcc_ir_generation_state* gen_state,
    struct gkcc_ir_translation_result prev_result,
    struct gkcc_ir_translation_result tr, struct ast_unary* unary);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_unary_bitwise_not(
    struct gkcc_ir_generation_state* gen_state,
    struct gkcc_ir_translation_result prev_result,
    struct gkcc_ir_translation_result tr, struct ast_unary* unary);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_return(
    struct gkcc_ir_generation_state* gen_state,
    struct ast_function_return* fn_return);

struct gkcc_ir_translation_result
gkcc_ir_translate_ast_node_binop_assign_multiply(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right);

struct gkcc_ir_translation_result
gkcc_ir_translate_ast_node_binop_assign_divide(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_assign_mod(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right);

struct gkcc_ir_translation_result gkcc_ir_translate_ast_node_binop_assign_add(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right);

struct gkcc_ir_translation_result
gkcc_ir_translate_ast_node_binop_assign_subtract(
    struct gkcc_ir_generation_state* gen_state, struct ast_binop* binop,
    struct gkcc_ir_translation_result tr,
    struct gkcc_ir_translation_result translation_result_left,
    struct gkcc_ir_translation_result translation_result_right);
#endif  // GKCC_TRANSLATORS_H

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

#include "quads.h"

#include <malloc.h>
#include <memory.h>

#include "ast/ast.h"
#include "ir/translators.h"

char *gkcc_ir_quad_register_constant_string(char *buf,
                                            struct gkcc_ir_quad_register *qr) {
  gkcc_assert(qr->register_type == GKCC_IR_QUAD_REGISTER_CONSTANT,
              GKCC_ERROR_INVALID_ARGUMENTS,
              "gkcc_ir_quad_register_constant_string() got a qr that is not of "
              "type GKCC_IR_QUAD_REGISTER_CONSTANT");

  switch (qr->constant->type) {
    case AST_CONSTANT_LONGLONG:
      sprintf(buf, "%lld", qr->constant->ylonglong);
      return buf;
    case AST_CONSTANT_LONG_DOUBLE:
      sprintf(buf, "%Lf", qr->constant->ylongdouble);
      return buf;
    case AST_CONSTANT_DOUBLE:
      sprintf(buf, "%lf", qr->constant->ydouble);
      return buf;
    case AST_CONSTANT_FLOAT:
      sprintf(buf, "%f", qr->constant->yfloat);
      return buf;
    case AST_CONSTANT_LONG:
      sprintf(buf, "%ld", qr->constant->ylong);
      return buf;
    case AST_CONSTANT_INT:
      sprintf(buf, "%d", qr->constant->yint);
      return buf;
    case AST_CONSTANT_CHAR:
      sprint_escaped_char(buf, qr->constant->ychar);
      return buf;
    case AST_CONSTANT_STRING:
      buf[0] = '"';
      sprint_escaped_string(&buf[1], qr->constant->ystring.raw,
                            qr->constant->ystring.length);
      int len = strlen(buf);
      buf[len] = '"';
      buf[len + 1] = '\0';
      return buf;
  }
  return NULL;
}

char *gkcc_ir_quad_register_string(char *buf,
                                   struct gkcc_ir_quad_register *qr) {
  char buf1[(1 << 12) + 1];

  if (qr == NULL) {
    strcpy(buf, "NULL");
    return buf;
  }

  switch (qr->register_type) {
    case GKCC_IR_QUAD_REGISTER_PSEUDOREGISTER:
      sprintf(buf, "%%T%d", qr->pseudoregister.register_num);
      break;
    case GKCC_IR_QUAD_REGISTER_SYMBOL:
      if (qr->symbol.is_global) {
        sprintf(buf, "global:%s", qr->symbol.symbol->symbol_name);
      } else {
        sprintf(buf, "local:%s", qr->symbol.symbol->symbol_name);
      }
      break;
    case GKCC_IR_QUAD_REGISTER_CONSTANT:
      sprintf(buf, "$%s", gkcc_ir_quad_register_constant_string(buf1, qr));
      break;
    case GKCC_IR_QUAD_REGISTER_BASIC_BLOCK:
      sprintf(buf, "%s", qr->basic_block->bb_name);
      break;
  }

  return buf;
}

void gkcc_ir_quad_print(struct gkcc_ir_quad_list *ql) {
  char buf1[(1 << 12) + 1];
  char buf2[(1 << 12) + 1];
  char buf3[(1 << 12) + 1];

  for (struct gkcc_ir_quad_list *n = ql; n != NULL; n = n->next) {
    struct gkcc_ir_quad *q = n->quad;
    printf("\t%s = %s %s %s\n", gkcc_ir_quad_register_string(buf1, q->dest),
           GKCC_IR_QUAD_INSTRUCTION_STRING[q->instruction],
           gkcc_ir_quad_register_string(buf2, q->source1),
           gkcc_ir_quad_register_string(buf3, q->source2));
  }
}

struct gkcc_ir_translation_result gkcc_ir_quad_generate_declaration(
    struct gkcc_ir_generation_state *gen_state, struct ast_node *node) {
  struct gkcc_ir_translation_result translation_result = {};
  int size = gkcc_type_sizeof(node->declaration.type->gkcc_type.gkcc_type);
  node->declaration.identifier->ident.symbol_table_entry->offset =
      gen_state->current_function->required_space_for_locals;
  gen_state->current_function->required_space_for_locals += size;

  return translation_result;
}

// Turns an AST into a list of quads
struct gkcc_ir_translation_result gkcc_ir_quad_generate_for_ast(
    struct gkcc_ir_generation_state *gen_state, struct ast_node *lnode) {
  switch (lnode->type) {
    case AST_NODE_UNKNOWN:
      gkcc_error_fatal(
          GKCC_ERROR_UNKNOWN_AST_TYPE,
          "Ran into an AST_NODE_UNKNOWN ast type during IR quad generation");
      break;
    case AST_NODE_BINOP:
      return gkcc_ir_translate_ast_node_binop(gen_state, lnode);
    case AST_NODE_CONSTANT:
      return gkcc_ir_translate_ast_constant(gen_state, &lnode->constant);
    case AST_NODE_IDENT:
      return gkcc_ir_translate_ast_ident(gen_state, &lnode->ident);
    case AST_NODE_UNARY:
      return gkcc_ir_translate_ast_node_unary(gen_state, &lnode->unary);
    case AST_NODE_TERNARY:
      break;
    case AST_NODE_GKCC_TYPE:
      break;
    case AST_NODE_DECLARATION:
      return gkcc_ir_quad_generate_declaration(gen_state, lnode);
    case AST_NODE_LIST:
      break;
    case AST_NODE_FUNCTION_CALL:
      return gkcc_ir_translate_ast_function_call(gen_state,
                                                 &lnode->function_call);
    case AST_NODE_ENUM_DEFINITION:
      break;
    case AST_NODE_FUNCTION_RETURN:
      return gkcc_ir_translate_ast_return(gen_state, &lnode->function_return);
    case AST_NODE_SWITCH_CASE_CASE:
      break;
    case AST_NODE_SWITCH_CASE_SWITCH:
      break;
    default:
      break;
  }
  gkcc_error_fatal(GKCC_ERROR_NOT_YET_IMPLEMENTED,
                   "gkcc_ir_quad_generate_for_ast() got to end of function "
                   "meaning the ast node was not yet implemented");
}

struct gkcc_ir_quad_register *gkcc_ir_quad_register_new_pseudoregister(
    struct gkcc_ir_generation_state *gen_state) {
  struct gkcc_ir_quad_register *gkcc_ir_quad_register =
      gkcc_ir_quad_register_new(GKCC_IR_QUAD_REGISTER_PSEUDOREGISTER);
  gkcc_ir_quad_register->pseudoregister.register_num =
      gen_state->current_pseudoregister_number++;

  return gkcc_ir_quad_register;
}

struct gkcc_ir_quad_register *gkcc_ir_quad_register_new_basic_block(
    struct gkcc_basic_block *bb) {
  struct gkcc_ir_quad_register *qr =
      gkcc_ir_quad_register_new(GKCC_IR_QUAD_REGISTER_BASIC_BLOCK);
  qr->basic_block = bb;

  return qr;
}

struct gkcc_ir_quad_register *gkcc_ir_quad_register_new(
    enum gkcc_ir_quad_register_type register_type) {
  struct gkcc_ir_quad_register *gkcc_register =
      malloc(sizeof(struct gkcc_ir_quad_register));
  memset(gkcc_register, 0, sizeof(struct gkcc_ir_quad_register));

  gkcc_register->register_type = register_type;
  gkcc_register->type = gkcc_type_new_signed_int_type();
  return gkcc_register;
}

struct gkcc_ir_quad_register *gkcc_ir_quad_register_new_int_constant(
    int constant) {
  struct gkcc_ir_quad_register *qr =
      gkcc_ir_quad_register_new(GKCC_IR_QUAD_REGISTER_CONSTANT);
  qr->constant = &ast_node_new_constant_int_node(constant)->constant;
  return qr;
}

struct gkcc_ir_quad *gkcc_ir_quad_new_with_args(
    enum gkcc_ir_quad_instruction instruction,
    struct gkcc_ir_quad_register *dest, struct gkcc_ir_quad_register *source1,
    struct gkcc_ir_quad_register *source2) {
  struct gkcc_ir_quad *quad = gkcc_ir_quad_new();
  quad->dest = dest;
  quad->instruction = instruction;
  quad->source1 = source1;
  quad->source2 = source2;
  return quad;
}

struct gkcc_ir_quad *gkcc_ir_quad_new(void) {
  struct gkcc_ir_quad *tr = malloc(sizeof(struct gkcc_ir_quad));
  memset(tr, 0, sizeof(struct gkcc_ir_quad));

  return tr;
}

struct gkcc_ir_quad_list *gkcc_ir_quad_list_append(
    struct gkcc_ir_quad_list *ql1, struct gkcc_ir_quad *ta) {
  struct gkcc_ir_quad_list *ql;
  if (ta == NULL) {
    return ql1;
  }
  if (ql1 == NULL) {
    ql = gkcc_ir_quad_list_new();
    ql->quad = ta;
    return ql;
  }
  for (ql = ql1; ql->next != NULL; ql = ql->next)
    ;
  ql->next = gkcc_ir_quad_list_new();
  ql->next->quad = ta;

  return ql1;
}

struct gkcc_ir_quad_list *gkcc_ir_quad_list_append_list(
    struct gkcc_ir_quad_list *ql1, struct gkcc_ir_quad_list *ql2) {
  if (ql1 == NULL) {
    return ql2;
  }
  if (ql2 == NULL) {
    return ql1;
  }

  struct gkcc_ir_quad_list *ql;
  for (ql = ql1; ql->next != NULL; ql = ql->next)
    ;
  ql->next = ql2;

  return ql1;
}

struct gkcc_ir_quad_list *gkcc_ir_quad_list_new(void) {
  struct gkcc_ir_quad_list *ql = malloc(sizeof(struct gkcc_ir_quad_list));
  memset(ql, 0, sizeof(struct gkcc_ir_quad_list));

  return ql;
}

struct gkcc_ir_generation_state *gkcc_ir_generation_state_new(void) {
  struct gkcc_ir_generation_state *gen_state =
      malloc(sizeof(struct gkcc_ir_generation_state));
  memset(gen_state, 0, sizeof(struct gkcc_ir_generation_state));

  return gen_state;
}

struct gkcc_ir_function *gkcc_ir_function_new(char *function_name) {
  struct gkcc_ir_function *ir_function =
      malloc(sizeof(struct gkcc_ir_function));
  memset(ir_function, 0, sizeof(struct gkcc_ir_function));

  ir_function->function_name = malloc(strlen(function_name) + 1);
  strcpy(ir_function->function_name, function_name);

  return ir_function;
}

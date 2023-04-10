// Copyright (C) 2023 Gary Kim <gary@garykim.dev>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef AST_H
#define AST_H

#include <stdbool.h>

#include "lex_extras.h"
#include "misc.h"
#include "types.h"

// ========================
// === struct ast_binop ===
// ========================

#define ENUM_AST_BINOP_TYPE(GEN)    \
  GEN(AST_BINOP_ADD)                \
  GEN(AST_BINOP_SUBTRACT)           \
  GEN(AST_BINOP_DIVIDE)             \
  GEN(AST_BINOP_MULTIPLY)           \
  GEN(AST_BINOP_MOD)                \
  GEN(AST_BINOP_ASSIGN)             \
  GEN(AST_BINOP_ASSIGN_MULTIPLY)    \
  GEN(AST_BINOP_ASSIGN_DIVIDE)      \
  GEN(AST_BINOP_ASSIGN_MOD)         \
  GEN(AST_BINOP_ASSIGN_ADD)         \
  GEN(AST_BINOP_ASSIGN_SUBTRACT)    \
  GEN(AST_BINOP_ASSIGN_BITWISE_SHL) \
  GEN(AST_BINOP_ASSIGN_BITWISE_SHR) \
  GEN(AST_BINOP_ASSIGN_BITWISE_AND) \
  GEN(AST_BINOP_ASSIGN_BITWISE_XOR) \
  GEN(AST_BINOP_ASSIGN_BITWISE_OR)  \
  GEN(AST_BINOP_SHL)                \
  GEN(AST_BINOP_SHR)                \
  GEN(AST_BINOP_EQUALS)             \
  GEN(AST_BINOP_LT)                 \
  GEN(AST_BINOP_GT)                 \
  GEN(AST_BINOP_LTEQ)               \
  GEN(AST_BINOP_GTEQ)               \
  GEN(AST_BINOP_BITWISE_AND)        \
  GEN(AST_BINOP_BITWISE_XOR)        \
  GEN(AST_BINOP_BITWISE_OR)         \
  GEN(AST_BINOP_LOGICAL_AND)        \
  GEN(AST_BINOP_LOGICAL_OR)

enum ast_binop_type { ENUM_AST_BINOP_TYPE(ENUM_VALUES) };

static const char* const AST_BINOP_TYPE_STRING[] = {
    ENUM_AST_BINOP_TYPE(ENUM_STRINGS)};

#undef ENUM_AST_BINOP_TYPE

struct ast_binop {
  enum ast_binop_type type;
  struct ast_node* left;
  struct ast_node* right;
};

// === AST_TERNARY ==
struct ast_ternary {
  struct ast_node* condition;
  struct ast_node* true_expr;
  struct ast_node* false_expr;
};

// ========================
// === struct ast_unary ===
// ========================

#define ENUM_AST_UNARY_TYPE(GEN) \
  GEN(AST_UNARY_SIZEOF)          \
  GEN(AST_UNARY_ADDRESSOF)       \
  GEN(AST_UNARY_DEREFERENCE)     \
  GEN(AST_UNARY_NOT)             \
  GEN(AST_UNARY_NEGATIVE)        \
  GEN(AST_UNARY_BITWISE_NOT)     \
  GEN(AST_UNARY_LOGICAL_NOT)

enum ast_unary_type { ENUM_AST_UNARY_TYPE(ENUM_VALUES) };

static const char* const AST_UNARY_TYPE_STRING[] = {
    ENUM_AST_UNARY_TYPE(ENUM_STRINGS)};

#undef ENUM_AST_UNARY_TYPE

struct ast_unary {
  enum ast_unary_type type;
  struct ast_node* of;
};

// ========================
// === struct ast_ident ===
// ========================

struct ast_ident {
  char* name;
};

// ============================
// === struct ast_gkcc_type ===
// ============================

struct ast_gkcc_type {
  struct gkcc_type* gkcc_type;
  struct ast_node* child;
};

// ===========================
// === struct ast_constant ===
// ===========================

enum ast_constant_type {
  AST_CONSTANT_LONGLONG = 1,
  AST_CONSTANT_LONG_DOUBLE,
  AST_CONSTANT_DOUBLE,
  AST_CONSTANT_FLOAT,
  AST_CONSTANT_LONG,
  AST_CONSTANT_INT,
  AST_CONSTANT_CHAR,
  AST_CONSTANT_STRING,
};

struct ast_constant {
  enum ast_constant_type type;
  bool is_unsigned;
  union {
    long long ylonglong;
    long double ylongdouble;
    double ydouble;
    float yfloat;
    long ylong;
    int yint;
    char ychar;
    struct ystring {
      char* raw;
      unsigned int length;
    } ystring;
  };
};

enum ast_node_type {
  AST_NODE_UNKNOWN,
  AST_NODE_BINOP,
  AST_NODE_CONSTANT,
  AST_NODE_IDENT,
  AST_NODE_UNARY,
  AST_NODE_TERNARY,
  AST_NODE_GKCC_TYPE,
};

struct ast_node {
  enum ast_node_type type;
  union {
    struct ast_binop binop;
    struct ast_constant constant;
    struct ast_unary unary;
    struct ast_ternary ternary;
    struct ast_gkcc_type gkcc_type;
    struct ast_ident ident;
  };
};

// =================
// === Functions ===
// =================

void ast_print(struct ast_node* top, int depth, char* prefix);
struct ast_node* ast_node_new(enum ast_node_type node_type);
void ast_node_string(char* buf, struct ast_node* node);
void yynum2ast_node(struct ast_node* node, struct _yynum* yynum);
struct ast_node* ast_node_new_binop_node(enum ast_binop_type,
                                         struct ast_node* left,
                                         struct ast_node* right);
const char* ast_binop_type_string(struct ast_binop* binop);
char* ast_constant_string(struct ast_constant*);
struct ast_node* ast_node_new_unary_node(enum ast_unary_type type,
                                         struct ast_node* of);
char* ast_unary_string(struct ast_unary* node);
struct ast_node* ast_node_new_ternary_node(struct ast_node* condition,
                                           struct ast_node* true_expr,
                                           struct ast_node* false_expr);
char* ast_ternary_string(struct ast_ternary* node);
struct ast_node* ast_node_new_gkcc_type_qualifier_node(
    enum gkcc_qualifier_type qualifier_type, struct ast_node* child);
struct ast_node* ast_node_new_constant_int_node(int val);
struct ast_node* yylval2ast_node(struct _yylval* yylval);
struct ast_node* ast_node_new_gkcc_storage_class_specifier_node(
    enum gkcc_storage_class_specifier_type type, struct ast_node* child);
struct ast_node* ast_node_new_gkcc_type_specifier_node(
    enum gkcc_type_specifier_type type, struct ast_node* child);
struct ast_node *yylval2ast_node_ident(struct _yylval *yylval);
char* ast_gkcc_type_string(struct ast_gkcc_type *gkcc_type);
#endif

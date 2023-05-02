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
  GEN(AST_BINOP_LOGICAL_OR)         \
  GEN(AST_BINOP_MEMBER_ACCESS)      \
  GEN(AST_BINOP_CAST)

enum ast_binop_type { ENUM_AST_BINOP_TYPE(ENUM_VALUES) };

static const char* const AST_BINOP_TYPE_STRING[] = {
    ENUM_AST_BINOP_TYPE(ENUM_STRINGS)};

#undef ENUM_AST_BINOP_TYPE

struct ast_binop {
  enum ast_binop_type type;
  struct ast_node* left;
  struct ast_node* right;
};

// =========================
// === struct ast_ternary ==
// =========================

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
  GEN(AST_UNARY_LOGICAL_NOT)     \
  GEN(AST_UNARY_POSTINC)         \
  GEN(AST_UNARY_POSTDEC)

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
  int length;
};

// ============================
// === struct ast_gkcc_type ===
// ============================

struct ast_gkcc_type {
  struct gkcc_type* gkcc_type;
};

// ==============================
// === struct ast_declaration ===
// ==============================

struct ast_declaration {
  struct ast_node* type;
  struct ast_node* identifier;
  struct ast_node* assignment;
  struct gkcc_symbol* symbol_table_entry;
};

// =======================
// === struct ast_list ===
// =======================

struct ast_list {
  struct ast_node* node;
  struct ast_node* next;
  struct ast_node* end;
};

// ============================
// === struct ast_top_level ===
// ============================

struct ast_top_level {
  struct ast_node* list;
};

// ==================================
// === struct ast_enum_definition ===
// ==================================

struct ast_enum_definition {
  struct ast_node* enumerators;
  struct ast_node* ident;
};

// ============================================
// === struct ast_struct_or_union_specifier ===
// ============================================

#define ENUM_AST_STRUCT_OR_UNION_SPECIFIER_TYPE(GEN) \
  GEN(AST_STRUCT_OR_UNION_DEFINITION_STRUCT)          \
  GEN(AST_STRUCT_OR_UNION_DEFINITION_UNION)

enum ast_struct_or_union_specifier_type {
  ENUM_AST_STRUCT_OR_UNION_SPECIFIER_TYPE(ENUM_VALUES)
};

static const char* const AST_STRUCT_OR_UNION_SPECIFIER_TYPE_STRING[] = {
    ENUM_AST_STRUCT_OR_UNION_SPECIFIER_TYPE(ENUM_STRINGS)};

#undef ENUM_AST_STRUCT_OR_UNION_DEFINITION_TYPE

struct ast_struct_or_union_specifier {
  enum ast_struct_or_union_specifier_type type;
  struct ast_node* ident;
};

// ================================
// === struct ast_function_call ===
// ================================

struct ast_function_call {
  struct ast_node* name;
  struct ast_node* parameters;
};

// ======================================
// === struct ast_function_definition ===
// ======================================

struct ast_function_definition {
  struct ast_node* name;
  struct ast_node* parameters;
  struct ast_node* returns;
  struct ast_node* statements;
};

// ===============================
// === struct ast_if_statement ===
// ===============================

struct ast_if_statement {
  struct ast_node* condition;
  struct ast_node* then_statement;
  struct ast_node* else_statement;
};

// ===========================
// === struct ast_for_loop ===
// ===========================

struct ast_for_loop {
  bool is_do_while;
  struct ast_node* expr1;
  struct ast_node* expr2;
  struct ast_node* expr3;
  struct ast_node* statements;
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

// =======================
// === struct ast_node ===
// =======================

#define ENUM_AST_NODE_TYPE(GEN)            \
  GEN(AST_NODE_UNKNOWN)                    \
  GEN(AST_NODE_BINOP)                      \
  GEN(AST_NODE_CONSTANT)                   \
  GEN(AST_NODE_IDENT)                      \
  GEN(AST_NODE_UNARY)                      \
  GEN(AST_NODE_TERNARY)                    \
  GEN(AST_NODE_GKCC_TYPE)                  \
  GEN(AST_NODE_DECLARATION)                \
  GEN(AST_NODE_LIST)                       \
  GEN(AST_NODE_TOP_LEVEL)                  \
  GEN(AST_NODE_FUNCTION_CALL)              \
  GEN(AST_NODE_FUNCTION_DEFINITION)        \
  GEN(AST_NODE_ENUM_DEFINITION)            \
  GEN(AST_NODE_STRUCT_OR_UNION_SPECIFIER) \
  GEN(AST_NODE_FOR_LOOP)                   \
  GEN(AST_NODE_IF_STATEMENT)

enum ast_node_type { ENUM_AST_NODE_TYPE(ENUM_VALUES) };

static const char* const AST_NODE_TYPE_STRING[] = {
    ENUM_AST_NODE_TYPE(ENUM_STRINGS)};

struct ast_node {
  enum ast_node_type type;
  union {
    struct ast_binop binop;
    struct ast_constant constant;
    struct ast_unary unary;
    struct ast_ternary ternary;
    struct ast_gkcc_type gkcc_type;
    struct ast_ident ident;
    struct ast_declaration declaration;
    struct ast_list list;
    struct ast_top_level top_level;
    struct ast_function_call function_call;
    struct ast_function_definition function_definition;
    struct ast_enum_definition enum_definition;
    struct ast_struct_or_union_specifier struct_or_union_specifier;
    struct ast_if_statement if_statement;
    struct ast_for_loop for_loop;
  };
};

// =================
// === Functions ===
// =================

void ast_print(struct ast_node* top, int depth, const char* prefix);
void ast_node_string(char* buf, struct ast_node* node, int depth);
void yynum2ast_node(struct ast_node* node, struct _yynum* yynum);
const char* ast_binop_type_string(struct ast_binop* binop);
char* ast_constant_string(struct ast_constant*);
char* ast_unary_string(struct ast_unary* node);
char* ast_ternary_string(struct ast_ternary* node);
struct ast_node* ast_node_new_constant_int_node(int val);
struct ast_node* yylval2ast_node(struct _yylval* yylval);
struct ast_node* ast_node_new_gkcc_type_specifier_node(
    enum gkcc_type_specifier_type type);
struct ast_node* yylval2ast_node_ident(struct _yylval* yylval);
void ast_gkcc_type_string(struct gkcc_type* gkcc_type, int depth);
struct ast_node* ast_node_append(struct ast_node* parent,
                                 struct ast_node* child);
struct ast_node *ast_node_direct_declarator_to_declarator(
    struct ast_node *original_node);
struct gkcc_type *ast_node_declaration_specifiers_to_gkcc_data_type(
    struct ast_node *declaration_specifiers);
#endif

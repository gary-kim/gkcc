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

#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast_constructors.h"
#include "lex_extras.h"
#include "misc.h"
#include "scope.h"

char flbuf[(1 << 16) + 1];

void ast_print(struct ast_node *top, int depth, const char *prefix) {
  char buf[(1 << 16) + 1];
  if (top == NULL) {
    return;
  }

  // Don't want the extra empty newline of AST_NODE_LIST
  if (prefix != NULL) {
    int start_of_line = 2 * depth;

    for (int i = 0; i < start_of_line; ++i) {
      buf[i] = ' ';
    }

    for (int i = 0; prefix[i] != '\0'; i++) {
      buf[start_of_line++] = prefix[i];
    }

    buf[start_of_line] = '\0';

    printf("%s", buf);

    ast_node_string(buf, top, depth + 1);

    if (buf[0] != '\0')  // Function has handled its own printing
      printf("%s\n", buf);
    else
      printf("\n");
  }

  switch (top->type) {
    case AST_NODE_BINOP:
      ast_print(top->binop.left, depth + 1, "expr 1: ");
      ast_print(top->binop.right, depth + 1, "expr 2: ");
      break;
    case AST_NODE_MEMBER_ACCESS:
      ast_print(top->member_access.struct_or_union, depth + 1, "in: ");
      ast_print(top->member_access.identifier, depth + 1, "access: ");
    case AST_NODE_CONSTANT:
      break;
    case AST_NODE_IDENT:
      gkcc_symbol_print_string(top->ident.symbol_table_entry, depth + 1);
      break;
    case AST_NODE_UNARY:
      ast_print(top->unary.of, depth + 1, "");
      break;
    case AST_NODE_TERNARY:
      ast_print(top->ternary.condition, depth + 1, "condition: ");
      ast_print(top->ternary.true_expr, depth + 1, "true_expr: ");
      ast_print(top->ternary.false_expr, depth + 1, "false_expr: ");
      break;
    case AST_NODE_DECLARATION:
      ast_print(top->declaration.identifier, depth + 1, "init_declarator: ");
      ast_print(top->declaration.type, depth + 1, "type: ");
      ast_print(top->declaration.assignment, depth + 1, "assignment: ");
      break;
    case AST_NODE_LIST:
      ast_print(top->list.node, depth + 1, "list_node: ");
      ast_print(top->list.next, depth, NULL);
      break;
    case AST_NODE_TOP_LEVEL:
      ast_print(top->top_level.list, depth, NULL);
      break;
    case AST_NODE_FUNCTION_CALL:
      ast_print(top->function_call.name, depth + 1, "function_name: ");
      ast_print(top->function_call.parameters, depth + 1, "parameters: ");
      break;
    case AST_NODE_ENUM_DEFINITION:
      ast_print(top->enum_definition.enumerators, depth + 1, "enumerators: ");
      ast_print(top->enum_definition.ident, depth + 1, "ident: ");
      break;
    case AST_NODE_STRUCT_OR_UNION_SPECIFIER:
      ast_print(top->struct_or_union_specifier.ident, depth + 1, "ident: ");
      break;
    case AST_NODE_IF_STATEMENT:
      ast_print(top->if_statement.condition, depth + 1, "condition: ");
      ast_print(top->if_statement.then_statement, depth + 1, "then: ");
      ast_print(top->if_statement.else_statement, depth + 1, "else: ");
      break;
    case AST_NODE_FOR_LOOP:
      ast_print(top->for_loop.expr1, depth + 1, "expr1: ");
      ast_print(top->for_loop.expr2, depth + 1, "expr2: ");
      ast_print(top->for_loop.expr3, depth + 1, "expr3: ");
      ast_print(top->for_loop.statements, depth + 1, "do: ");
      break;
    case AST_NODE_FUNCTION_RETURN:
      ast_print(top->function_return.to_return, depth + 1, "returning: ");
      break;
    case AST_NODE_SWITCH_CASE_CASE:
      ast_print(top->switch_case_case.expression, depth + 1, "expression: ");
      ast_print(top->switch_case_case.statement, depth + 1, "statements: ");
      break;
    case AST_NODE_SWITCH_CASE_SWITCH:
      ast_print(top->switch_case_switch.expression, depth + 1,
                "constant_expression: ");
      ast_print(top->switch_case_switch.statements, depth + 1, "statements: ");
      break;
    default:
      break;
  }
}

void ast_node_string(char *buf, struct ast_node *node, int depth) {
  char extrabuf[8193];
  switch (node->type) {
    case AST_NODE_BINOP:
      sprintf(buf, "%s: %s", AST_NODE_TYPE_STRING[AST_NODE_BINOP],
              ast_binop_type_string(&node->binop));
      break;
    case AST_NODE_CONSTANT:
      strcpy(buf, ast_constant_string(&node->constant));
      break;
    case AST_NODE_IDENT:
      sprint_escaped_string(extrabuf, node->ident.name, node->ident.length);
      sprintf(buf, "%s: %s", AST_NODE_TYPE_STRING[AST_NODE_IDENT], extrabuf);
      break;
    case AST_NODE_UNARY:
      strcpy(buf, ast_unary_string(&node->unary));
      break;
    case AST_NODE_TERNARY:
      strcpy(buf, ast_ternary_string(&node->ternary));
      break;
    case AST_NODE_GKCC_TYPE:
      printf("\n");
      buf[0] = '\0';
      ast_gkcc_type_string(node->gkcc_type.gkcc_type, depth, "");
      break;
    case AST_NODE_LIST:
      buf[0] = '\0';
      break;
    case AST_NODE_STRUCT_OR_UNION_SPECIFIER:
      sprintf(buf, "%s (type=%s): ", AST_NODE_TYPE_STRING[node->type],
              AST_STRUCT_OR_UNION_SPECIFIER_TYPE_STRING
                  [node->struct_or_union_specifier.type]);
      break;
    case AST_NODE_FOR_LOOP:
      if (node->for_loop.is_do_while)
        strcpy(extrabuf, "true");
      else
        strcpy(extrabuf, "false");
      sprintf(buf, "%s (is_do_while=%s):", AST_NODE_TYPE_STRING[node->type],
              extrabuf);
      break;
    case AST_NODE_GOTO_NODE:
      sprintf(buf, "%s: %s", AST_NODE_TYPE_STRING[node->type],
              node->goto_node.ident->ident.name);
      break;
    default:
      sprintf(buf, "%s:", AST_NODE_TYPE_STRING[node->type]);
      break;
  }
}

const char *ast_binop_type_string(struct ast_binop *binop) {
  return AST_BINOP_TYPE_STRING[binop->type];
}

void print_to_depth(int depth) {
  for (int i = 0; i < depth; i++) {
    printf("  ");
  }
}

void ast_gkcc_type_string(struct gkcc_type *gkcc_type, int depth,
                          char *prefix) {
  if (gkcc_type == NULL) return;
  const char *type_type = GKCC_TYPE_TYPE_STRING[gkcc_type->type];
  int start_of_line = 0;
  for (int i = 0; i < 2 * depth; i++) {
    flbuf[start_of_line++] = ' ';
  }
  char *writeloc = &flbuf[start_of_line];
  if (gkcc_type->ident != NULL) {
    writeloc += sprintf(
        writeloc, "%sGKCC_TYPE (type=%s, scope=%s, stg_class=%s): ", prefix,
        type_type,
        GKCC_SCOPE_STRING[gkcc_type->ident->ident.symbol_table_entry
                              ->symbol_table_set->scope],
        GKCC_STORAGE_CLASS_STRING[gkcc_type->ident->ident.symbol_table_entry
                                      ->storage_class]);
  } else {
    writeloc += sprintf(writeloc, "%sGKCC_TYPE (type=%s): ", prefix, type_type);
  }

  switch (gkcc_type->type) {
    case GKCC_TYPE_FUNCTION:
      printf("%s\n", flbuf);
      ast_gkcc_type_string(gkcc_type->function_declaration.return_type,
                           depth + 1, "returns: ");
      ast_print(gkcc_type->function_declaration.statements, depth + 1,
                "statements:");
      break;
    case GKCC_TYPE_ARRAY:
      printf("%s\n", flbuf);
      ast_print(gkcc_type->array.size, depth + 1, "size: ");
      break;
    case GKCC_TYPE_QUALIFIER:
    case GKCC_TYPE_STORAGE_CLASS_SPECIFIER:
    case GKCC_TYPE_TYPE_SPECIFIER:
      sprintf(writeloc, "%s\n",
              GKCC_TYPE_SPECIFIER_TYPE_STRING[gkcc_type->type_specifier.type]);
      printf("%s", flbuf);
      break;
    case GKCC_TYPE_STRUCT:
    case GKCC_TYPE_UNION:
      if (gkcc_type->symbol_table_set->general_namespace->symbol_list == NULL) {
        sprintf(
            writeloc, "'%s' defined at %s:%d\n", gkcc_type->ident->ident.name,
            gkcc_type->ident->ident.symbol_table_entry->filename,
            gkcc_type->ident->ident.symbol_table_entry->effective_line_number);
        printf("%s", flbuf);
        break;
      }
      sprintf(
          writeloc, "'%s' defined at %s:%d with members {\n",
          gkcc_type->ident->ident.name,
          gkcc_type->ident->ident.symbol_table_entry->filename,
          gkcc_type->ident->ident.symbol_table_entry->effective_line_number);
      printf("%s", flbuf);
      gkcc_symbol_table_print(gkcc_type->symbol_table_set->general_namespace,
                              depth + 1);
      print_to_depth(depth);
      printf("}\n");
      break;
    default:
      printf("%s\n", flbuf);
      break;
  }

  ast_gkcc_type_string(gkcc_type->of, depth + 1, "");

  flbuf[0] = '\0';
}

char *ast_constant_string(struct ast_constant *constant) {
  char local_buf[10000];
  switch (constant->type) {
    case AST_CONSTANT_LONGLONG:
      sprintf(flbuf, "CONSTANT: (type=LONG LONG) %lld", constant->ylonglong);
      break;
    case AST_CONSTANT_LONG_DOUBLE:
      sprintf(flbuf, "CONSTANT: (type=LONG DOUBLE) %Lf", constant->ylongdouble);
      break;
    case AST_CONSTANT_DOUBLE:
      sprintf(flbuf, "CONSTANT: (type=DOUBLE) %f", constant->ydouble);
      break;
    case AST_CONSTANT_FLOAT:
      sprintf(flbuf, "CONSTANT: (type=FLOAT) %f", constant->yfloat);
      break;
    case AST_CONSTANT_LONG:
      sprintf(flbuf, "CONSTANT: (type=LONG) %ld", constant->ylong);
      break;
    case AST_CONSTANT_INT:
      sprintf(flbuf, "CONSTANT: (type=INT) %d", constant->yint);
      break;
    case AST_CONSTANT_CHAR:
      sprint_escaped_char(local_buf, constant->ychar);
      sprintf(flbuf, "CONSTANT: (type=CHAR) '%s'", local_buf);
      break;
    case AST_CONSTANT_STRING:
      sprint_escaped_string(local_buf, constant->ystring.raw,
                            constant->ystring.length);
      sprintf(flbuf, "CONSTANT: (type=STRING) \"%s\"", local_buf);
      break;
  }
  return flbuf;
}

char *ast_unary_string(struct ast_unary *node) {
  sprintf(flbuf, "UNARY OP %s", AST_UNARY_TYPE_STRING[node->type]);
  return flbuf;
}

char *ast_ternary_string(struct ast_ternary *node) {
  sprintf(flbuf, "TERNARY:");
  return flbuf;
}

struct ast_node *ast_node_new_constant_int_node(int val) {
  struct ast_node *node = ast_node_new(AST_NODE_CONSTANT);
  node->constant.type = AST_CONSTANT_INT;
  node->constant.yint = val;
  return node;
}

void yynum2ast_node(struct ast_node *node, struct _yynum *yynum) {
  node->type = AST_NODE_CONSTANT;
  node->constant.is_unsigned = yynum->is_unsigned;
  switch (yynum->type) {
    case YYNUM_TYPE_LONGLONG:
      node->constant.type = AST_CONSTANT_LONGLONG;
      node->constant.ylonglong = yynum->num.ylonglong;
      break;
    case YYNUM_TYPE_LONG_DOUBLE:
      node->constant.type = AST_CONSTANT_LONG_DOUBLE;
      node->constant.ylongdouble = yynum->num.ylongdouble;
      break;
    case YYNUM_TYPE_DOUBLE:
      node->constant.type = AST_CONSTANT_DOUBLE;
      node->constant.ydouble = yynum->num.ydouble;
      break;
    case YYNUM_TYPE_FLOAT:
      node->constant.type = AST_CONSTANT_FLOAT;
      node->constant.yfloat = yynum->num.yfloat;
      break;
    case YYNUM_TYPE_LONG:
      node->constant.type = AST_CONSTANT_LONG;
      node->constant.ylong = yynum->num.ylong;
      break;
    case YYNUM_TYPE_INT:
      node->constant.type = AST_CONSTANT_INT;
      node->constant.yint = yynum->num.yint;
      break;
  }
}

struct ast_node *ast_node_append(struct ast_node *parent,
                                 struct ast_node *child) {
  // if parent == NULL, the user probably wanted to create a new list node
  // from scratch. Do it for them.
  if (parent == NULL) {
    return ast_node_new_list_node(child);
  }

  // Sometimes, the child is NULL because there is an attempt to append a
  // expression or other ast_node that actually turned out to be empty.
  // In that case, just return the parent directly.
  if (child == NULL) return parent;

  gkcc_assert(parent->type == AST_NODE_LIST, GKCC_ERROR_INVALID_ARGUMENTS,
              "ast_node_append given an ast_node that is not AST_NODE_LIST");
  struct ast_node *last_node = parent->list.end;
  struct ast_node *list_child = NULL;

  if (child->type == AST_NODE_LIST) {
    // Flatten the list
    for (struct ast_node *n = child; n != NULL; n = n->list.next) {
      // Sometimes, list nodes of nothing are made for compatability reasons
      // Throw it away at this stage.
      if (n->list.node == NULL) continue;
      list_child = ast_node_new_list_node(n->list.node);
      last_node->list.next = list_child;
      last_node = list_child;
    }
  } else {
    list_child = ast_node_new_list_node(child);
    last_node->list.next = list_child;
  }
  parent->list.end = list_child;
  return parent;
}

// ast_node_prepend is similar to ast_node_append but prepends the child rather
// then appending it.
// The function actually calls ast_node_append with arguments flipped but
// with some work done to make sure that the assumptions made by ast_node_append
// do not get violated by the prepend.
struct ast_node *ast_node_prepend(struct ast_node *parent,
                                  struct ast_node *child) {
  return ast_node_append(child, parent);
}

struct ast_node *ast_node_new_gkcc_type_specifier_node(
    enum gkcc_type_specifier_type type) {
  struct ast_node *node = ast_node_new(AST_NODE_GKCC_TYPE);
  node->gkcc_type.gkcc_type = gkcc_type_new(GKCC_TYPE_TYPE_SPECIFIER);
  node->gkcc_type.gkcc_type->type = GKCC_TYPE_TYPE_SPECIFIER;
  node->gkcc_type.gkcc_type->type_specifier.type = type;
  return node;
}

struct ast_node *ast_node_direct_declarator_to_declarator(
    struct ast_node *original_node) {
  if (original_node->type != AST_NODE_LIST) {
    original_node = ast_node_new_list_node(original_node);
  }

  gkcc_assert(original_node->list.node->type == AST_NODE_DECLARATION,
              GKCC_ERROR_INVALID_ARGUMENTS,
              "ast_node_direct_declarator_to_declarator got an original_node "
              "that is not of type AST_NODE_DECLARATION");

  struct ast_declaration *declaration_node =
      &original_node->list.node->declaration;

  struct ast_node *new_declaration_type = NULL;

  for (struct ast_node *n_list = original_node->list.next; n_list != NULL;
       n_list = n_list->list.next) {
    struct ast_node *n = n_list->list.node;

    gkcc_assert(n->type == AST_NODE_GKCC_TYPE, GKCC_ERROR_UNEXPECTED_VALUE,
                "ast_node_direct_declarator_to_declarator got a node that is "
                "not of type AST_NODE_GKCC_TYPE");

    if (new_declaration_type == NULL) {
      new_declaration_type = n;
      continue;
    }

    new_declaration_type->gkcc_type.gkcc_type = gkcc_type_append(
        new_declaration_type->gkcc_type.gkcc_type, n->gkcc_type.gkcc_type);
  }
  if (new_declaration_type != NULL) {
    new_declaration_type->gkcc_type.gkcc_type =
        gkcc_type_append(declaration_node->type->gkcc_type.gkcc_type,
                         new_declaration_type->gkcc_type.gkcc_type);
    declaration_node->type = new_declaration_type;
  }
  return original_node->list.node;
}

struct ast_node *ast_node_identifier_set_symbol_if_exists(
    struct gkcc_symbol_table_set *symbol_table_set, struct ast_node *node,
    enum gkcc_namespace namespace) {
  gkcc_assert(node->type == AST_NODE_IDENT, GKCC_ERROR_INVALID_ARGUMENTS,
              "ast_node_identifier_set_symbol_if_exists() got a node that is "
              "not of type AST_NODE_IDENT");

  node->ident.symbol_table_entry = gkcc_symbol_table_set_get_symbol(
      symbol_table_set, node->ident.name, namespace, true);

  return node;
}

enum gkcc_error ast_node_identifier_verify_symbol_exists(struct ast_node *node,
                                                         char *filename,
                                                         int line_number) {
  gkcc_assert(node->type == AST_NODE_IDENT, GKCC_ERROR_INVALID_ARGUMENTS,
              "ast_node_identifier_verify_symbol_exists() got a node that is "
              "not of type AST_NODE_IDENT");

  char buf[(1 << 12) + 1];
  sprintf(buf,
          "Attempt to use an identifier '%s' that cannot be found at %s:%d",
          node->ident.name, filename, line_number);

  gkcc_assert(node->ident.symbol_table_entry != NULL,
              GKCC_ERROR_CANNOT_FIND_SYMBOL, buf);

  return GKCC_ERROR_SUCCESS;
}

struct gkcc_type *ast_node_declaration_specifiers_to_gkcc_data_type(
    struct ast_node *declaration_specifiers) {
  // Figure out the signedness
  bool signed_figured_out = false;
  bool is_signed = true;
  for (struct ast_node *lc = declaration_specifiers; lc != NULL;
       lc = lc->list.next) {
    struct gkcc_type *current_type = lc->list.node->gkcc_type.gkcc_type;
    if (current_type->type != GKCC_TYPE_TYPE_SPECIFIER) continue;

    if (current_type->type_specifier.type == GKCC_TYPE_SPECIFIER_SIGNED) {
      if (signed_figured_out) {
        gkcc_error_fatal(GKCC_ERROR_REDECLARATION,
                         "Redeclaration of signedness not allowed");
      }
      signed_figured_out = true;
      continue;
    }

    if (current_type->type_specifier.type == GKCC_TYPE_SPECIFIER_UNSIGNED) {
      if (signed_figured_out) {
        gkcc_error_fatal(GKCC_ERROR_REDECLARATION,
                         "Redeclaration of signedness not allowed");
      }
      signed_figured_out = true;
      is_signed = false;
      continue;
    }
  }

  struct gkcc_type *type = is_signed ? gkcc_type_new(GKCC_TYPE_SIGNED)
                                     : gkcc_type_new(GKCC_TYPE_UNSIGNED);

  struct gkcc_type *complex_type = NULL;

  // Figure out data type
  // This part is sort of a disaster because it's basically an entire state
  // machine

#define CHECK_AND_SET_TYPE(type)                                         \
  if (gkcc_type_type != GKCC_TYPE_UNKNOWN)                               \
    gkcc_error_fatal(GKCC_ERROR_CONFLICT, "conflicting datatype found"); \
  gkcc_type_type = type;

  enum gkcc_type_type gkcc_type_type = GKCC_TYPE_UNKNOWN;
  for (struct ast_node *lc = declaration_specifiers; lc != NULL;
       lc = lc->list.next) {
    struct gkcc_type *current_type = lc->list.node->gkcc_type.gkcc_type;
    if (current_type->type != GKCC_TYPE_TYPE_SPECIFIER) continue;

    switch (current_type->type_specifier.type) {
      case GKCC_TYPE_SPECIFIER_VOID:
        CHECK_AND_SET_TYPE(GKCC_TYPE_SCALAR_VOID);
        break;
      case GKCC_TYPE_SPECIFIER_CHAR:
        CHECK_AND_SET_TYPE(GKCC_TYPE_SCALAR_CHAR);
        break;
      case GKCC_TYPE_SPECIFIER_SHORT:
        CHECK_AND_SET_TYPE(GKCC_TYPE_SCALAR_SHORT);
        break;
      case GKCC_TYPE_SPECIFIER_INT:
        if (gkcc_type_type == GKCC_TYPE_UNKNOWN) {
          gkcc_type_type = GKCC_TYPE_SCALAR_INT;
          continue;
        }
        if (gkcc_type_type == GKCC_TYPE_SCALAR_LONG ||
            gkcc_type_type == GKCC_TYPE_SCALAR_LONGLONG ||
            gkcc_type_type == GKCC_TYPE_SCALAR_SHORT) {
          // This is a noop
          continue;
        }
        gkcc_error_fatal(GKCC_ERROR_CONFLICT, "conflicting datatype found");
        break;
      case GKCC_TYPE_SPECIFIER_LONG:
        if (gkcc_type_type == GKCC_TYPE_UNKNOWN) {
          gkcc_type_type = GKCC_TYPE_SCALAR_LONG;
          continue;
        }

        if (gkcc_type_type == GKCC_TYPE_SCALAR_LONG) {
          gkcc_type_type = GKCC_TYPE_SCALAR_LONGLONG;
          continue;
        }

        if (gkcc_type_type == GKCC_TYPE_SCALAR_DOUBLE) {
          gkcc_type_type = GKCC_TYPE_SCALAR_LONG_DOUBLE;
          continue;
        }

        gkcc_error_fatal(GKCC_ERROR_CONFLICT, "conflicting datatype found");
        break;
      case GKCC_TYPE_SPECIFIER_FLOAT:
        CHECK_AND_SET_TYPE(GKCC_TYPE_SCALAR_FLOAT);
        break;
      case GKCC_TYPE_SPECIFIER_DOUBLE:
        CHECK_AND_SET_TYPE(GKCC_TYPE_SCALAR_DOUBLE);
        break;
      case GKCC_TYPE_SPECIFIER_STRUCT:
        CHECK_AND_SET_TYPE(GKCC_TYPE_STRUCT);
        complex_type = current_type;
        break;
      case GKCC_TYPE_SPECIFIER_UNION:
        CHECK_AND_SET_TYPE(GKCC_TYPE_UNION);
        complex_type = current_type;
        break;
      case GKCC_TYPE_SPECIFIER_ENUM:
        CHECK_AND_SET_TYPE(GKCC_TYPE_ENUM);
        break;
      default:
        break;
    }
  }

  struct gkcc_type *new_type = gkcc_type_new(gkcc_type_type);
  new_type->symbol_table_set =
      (complex_type != NULL) ? complex_type->symbol_table_set : NULL;
  new_type->ident = (complex_type != NULL) ? complex_type->ident : NULL;
  if (gkcc_is_gkcc_type_scalar(new_type)) {
    type = gkcc_type_append(type, new_type);
  } else {
    type = new_type;
  }

  return type;
}

struct ast_node *yylval2ast_node_ident(struct _yylval *yylval) {
  struct ast_node *node = ast_node_new(AST_NODE_IDENT);
  gkcc_assert(yylval->type == YYLVAL_TYPE_STRING, GKCC_ERROR_INVALID_ARGUMENTS,
              "yylval2ast_node_ident() was given a non-string yylval");

  node->ident.length = yylval->data.string.length;
  node->ident.name = malloc(node->ident.length + 1);
  memcpy(node->ident.name, yylval->data.string.string, node->ident.length);
  node->ident.name[node->ident.length] = '\0';

  return node;
}

struct ast_node *yylval2ast_node(struct _yylval *yylval) {
  struct ast_node *node = ast_node_new(AST_NODE_UNKNOWN);
  switch (yylval->type) {
    case YYLVAL_TYPE_NUMBER:
      yynum2ast_node(node, &yylval->data.number);
      break;
    case YYLVAL_TYPE_CHAR:
      node->type = AST_NODE_CONSTANT;
      node->constant.type = AST_CONSTANT_CHAR;
      node->constant.ychar = yylval->data.character;
      break;
    case YYLVAL_TYPE_STRING:
      node->type = AST_NODE_CONSTANT;
      node->constant.type = AST_CONSTANT_STRING;

      node->constant.ystring.length = yylval->data.string.length;
      node->constant.ystring.raw = malloc(node->constant.ystring.length);
      memcpy(node->constant.ystring.raw, yylval->data.string.string,
             node->constant.ystring.length);
      break;
  }
  return node;
}

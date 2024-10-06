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

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"
#include "c.tab.h"
#include "ir/basic_block.h"
#include "ir/ir_full.h"
#include "misc/misc.h"
#include "target_code/x86.h"

enum jobs {
  JOB_BUILD_NOTHING = 0,
  JOB_BUILD_AST,
  JOB_BUILD_BB,
  JOB_BUILD_IR,
  JOB_BUILD_ASSEMBLY,
  JOB_MAX,
};

int main(int argc, char** argv) {
  setup_segfault_stack_trace();

  bool should_print_ast = false;
  bool should_print_ir = false;
  FILE* out_file = stdout;
  int nsecs = 0;
  int flags = 0;
  int tfnd = 0;
  int opt = 0;

  while ((opt = getopt(argc, argv, "adio:")) != -1) {
    switch (opt) {
      case 'a':
        should_print_ast = true;
        break;
      case 'd':
        yydebug = 1;
        break;
      case 'i':
        should_print_ir = true;
        break;
      case 'o':
        if ((strcmp("-", optarg) == 0) || (strcmp("stdout", optarg) == 0)) {
          out_file = stdout;
          break;
        }
        out_file = fopen(optarg, "w");
        break;
      default:
        fprintf(stderr, "Cannot parse flags\n");
        return 255;
    }
  }

  enum jobs jobs = 0;
  if (optind <= argc) {
    jobs = JOB_MAX - 1;
  } else if (strcmp("ast", argv[optind]) == 0) {
    jobs = JOB_BUILD_AST;
  } else if (strcmp("ir", argv[optind]) == 0) {
    jobs = JOB_BUILD_IR;
  } else if (strcmp("assembly", argv[optind]) == 0) {
    jobs = JOB_BUILD_ASSEMBLY;
  }

  if (jobs < JOB_BUILD_AST) {
    return 0;
  }

  struct ast_node ast_node;
  struct gkcc_symbol_table_set* global_symbol_table =
      gkcc_symbol_table_set_new(NULL, GKCC_SCOPE_GLOBAL);

  yyparse(&ast_node, global_symbol_table);

  struct ast_node* top_level = ast_node_new(AST_NODE_TOP_LEVEL);
  top_level->top_level.list = &ast_node;

  if (should_print_ast) {
    printf(
        "============================\n"
        "=== Abstract Syntax Tree ===\n"
        "============================\n\n");
    ast_print(top_level, 0, "");
  }

  if (jobs < JOB_BUILD_BB) {
    return 0;
  }

  struct gkcc_ir_full* ir_full = gkcc_ir_build_full(top_level);

  if (should_print_ir) {
    printf(
        "=========================================\n"
        "=== Intermediate Representation QUADS ===\n"
        "=========================================\n\n");
    gkcc_ir_full_print(ir_full);
  }

  if (jobs < JOB_BUILD_ASSEMBLY) {
    return 0;
  }

  gkcc_tx86_generate_ir_full(out_file, ir_full);
}

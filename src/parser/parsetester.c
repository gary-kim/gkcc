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

#include "parsetester.h"

#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "ast_constructors.h"
#include "c.tab.h"
#include "scope.h"

int main(int argc, char** argv) {
  setup_segfault_stack_trace();

  if (argc <= 1) {
    printf("Nothing to do. EXITING\n");
    return 0;
  }

  if (strcmp(argv[1], "debug") == 0) yydebug = 1;

  struct ast_node ast_node;
  struct gkcc_symbol_table_set* global_symbol_table =
      gkcc_symbol_table_set_new(NULL, GKCC_SCOPE_GLOBAL);

  yyparse(&ast_node, global_symbol_table);

  struct ast_node *top_level = ast_node_new(AST_NODE_TOP_LEVEL);
  top_level->top_level.list = &ast_node;

  ast_print(top_level, 0, "");
}

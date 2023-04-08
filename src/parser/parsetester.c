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
#include "c.tab.h"

int main(int argc, char** argv) {

  if (argc <= 1) {
    printf("Nothing to do. EXITING\n");
    return 0;
  }

  if (strcmp(argv[1], "debug") == 0)
    yydebug = 1;

  struct ast_node ast_node;

  yyparse(&ast_node);

  print_nodes(&ast_node, 0);
}

void print_nodes(struct ast_node* top, int depth) {
  char buf[1024];
  if (top == NULL) {
    return;
  }

  int start_of_line = 2 * depth;

  for (int i = 0; i < start_of_line; ++i) {
    buf[i] = ' ';
  }

  ast_node_string(&buf[start_of_line], top);

  printf("%s\n", buf);

  switch (top->type) {
  case AST_NODE_BINOP:
    print_nodes(top->binop.left, depth + 1);
    print_nodes(top->binop.right, depth + 1);
    break;
  }
}

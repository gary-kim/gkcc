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

#ifndef GKCC_SCOPE_HELPERS_H
#define GKCC_SCOPE_HELPERS_H

#include "scope/scope.h"

// =============================
// === Function Declarations ===
// =============================

enum gkcc_error gkcc_scope_add_variable_to_scope(
    struct gkcc_symbol_table_set *symbol_table_set,
    struct ast_node *declaration_list, int line_number, char *file_name);

#endif  // GKCC_SCOPE_HELPERS_H

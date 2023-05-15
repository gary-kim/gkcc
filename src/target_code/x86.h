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

#ifndef GKCC_X86_H
#define GKCC_X86_H

#include <stdio.h>

#include "ir/quads.h"

// =============================
// === FUNCTION DECLARATIONS ===
// =============================

char *gkcc_tx86_translate_ir_quad_register(char *buf,
                                           struct gkcc_ir_quad_register *qr);

void gkcc_tx86_translate_ir_quad(FILE *out_file, struct gkcc_ir_quad *quad);

void gkcc_tx86_print_bb(FILE *out_file, bool *printed, struct gkcc_basic_block *bb);

void gkcc_tx86_generate_ir_full(FILE *out_file, struct gkcc_ir_full *ir_full);

#endif  // GKCC_X86_H

#ifndef GKCC_IR_BASE_H
#define GKCC_IR_BASE_H

#include <stdbool.h>

// =======================================
// === struct gkcc_ir_generation_state ===
// =======================================

struct gkcc_ir_generation_state {
  int current_pseudoregister_number;
  int current_basic_block_number;
  int current_string_constant_number;
  struct gkcc_ir_function *current_function;
  struct gkcc_ir_full *ir_full;
};

// =============================
// === struct gkcc_ir_symbol ===
// =============================

struct gkcc_ir_symbol {
  bool is_global;
  struct ystring *ystring;
  struct gkcc_symbol *symbol;
};

#endif

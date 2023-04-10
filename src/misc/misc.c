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

#include "misc.h"
#include <stdio.h>
#include <sys/signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#ifdef __SANITIZE_ADDRESS__
#include <sanitizer/asan_interface.h>
#endif

#include "dmezh/backtrace.h"

const char* gkcc_report_error(enum gkcc_error error, char* reporter) {
  fprintf(stderr, "ERROR (%s): %s\n", reporter, GKCC_ERROR_STRING[error]);
}

inline void gkcc_assert(int to_assert, char* message) {
  if (to_assert)
    return;

  die(message);
}

void gkcc_error_fatal(enum gkcc_error err, char* message) {
  char error_message[4096];
  sprintf(error_message, "(%s) %s", GKCC_ERROR_STRING[err], message);
  die(error_message);
}

void segfault_handler() {
  die("Caught segfault");
}

// Disable ASAN leak checker
#ifdef __SANITIZE_ADDRESS__
const char *__asan_default_options() {
  return "detect_leaks=0";
}
#endif

void setup_segfault_stack_trace() {
  struct sigaction sa;
  sa.sa_handler = segfault_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGSEGV, &sa, NULL);
}

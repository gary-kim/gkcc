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
#include <string.h>

#ifdef __SANITIZE_ADDRESS__
#include <sanitizer/asan_interface.h>
#endif

#include "dmezh/backtrace.h"

void gkcc_report_error(enum gkcc_error error, char* reporter) {
  fprintf(stderr, "ERROR (%s): %s\n", reporter, GKCC_ERROR_STRING[error]);
}

void gkcc_assert(int to_assert, enum gkcc_error err, char* message) {
  if (to_assert)
    return;

  gkcc_report_error(err, "gkcc_assert");
  die(message);
}

void gkcc_assert_success(enum gkcc_error err, char* message) {
    gkcc_assert(err == GKCC_ERROR_SUCCESS, err, message);
}

void gkcc_error_fatal(enum gkcc_error err, char* message) {
  char error_message[4096];
  sprintf(error_message, "(%s) %s", GKCC_ERROR_STRING[err], message);
  die(error_message);
}

void segfault_handler(void) {
  die("Caught segfault");
}

// Disable ASAN leak checker
#ifdef __SANITIZE_ADDRESS__
const char *__asan_default_options(void) {
  return "detect_leaks=0";
}
#endif

void setup_segfault_stack_trace(void) {
  struct sigaction sa;
  sa.sa_handler = (__sighandler_t)segfault_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGSEGV, &sa, NULL);
}

void print_escaped_char(char toprint) {
  char buf[16];
  sprint_escaped_char(buf, toprint);
  printf("'%s'", buf);
}

void print_escaped_string(char *str, size_t len) {
  char buf[8193];
  sprint_escaped_string(buf, str, len);
  printf("\"%s\"", buf);
}

void sprint_escaped_string(char* buf, char* str, size_t len) {
  size_t pos = 0;
  for (int i = 0; i < len; i++) {
    pos += sprint_escaped_char(&buf[pos], str[i]) - 1;
  }
  buf[pos] = '\0';
}

// Returns used up length
int sprint_escaped_char(char* buf, char toprint) {
  switch (toprint) {
    case '\'':
      strcpy(buf, "\\'");
      return sizeof("\\'");
    case '"':
      strcpy(buf, "\\\"");
      return sizeof("\\\"");
    case '\\':
      strcpy(buf, "\\\\");
      return sizeof("\\\\");
  }

  if (isprint(toprint)) {
    buf[0] = toprint;
    return 2;
  }

  switch (toprint) {
    case '\0':
      strcpy(buf, "\\0");
      return sizeof("\\0");
    case '\a':
      strcpy(buf, "\\a");
      return sizeof("\\a");
    case '\b':
      strcpy(buf, "\\b");
      return sizeof("\\b");
    case '\f':
      strcpy(buf, "\\f");
      return sizeof("\\f");
    case '\n':
      strcpy(buf, "\\n");
      return sizeof("\\n");
    case '\r':
      strcpy(buf, "\\r");
      return sizeof("\\r");
    case '\t':
      strcpy(buf, "\\t");
      return sizeof("\\t");
    case '\v':
      strcpy(buf, "\\v");
      return sizeof("\\v");
    default:
      return sprintf(buf, "\\%03o", (unsigned char) toprint);
  }
}

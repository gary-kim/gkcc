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

#ifndef GKCC_MISC_H
#define GKCC_MISC_H

#define ENUM_VALUES(VALUE) VALUE,
#define ENUM_STRINGS(STRING) #STRING,

#define ENUM_GKCC_ERROR(GEN) \
  GEN(GKCC_ERROR_SUCCESS)                           \
  GEN(GKCC_ERROR_UNKNOWN)           \
  GEN(GKCC_ERROR_INVALID_ARGUMENTS) \
  GEN(GKCC_ERROR_SYMBOL_ALREADY_EXISTS)

enum gkcc_error { ENUM_GKCC_ERROR(ENUM_VALUES) };

static const char* const GKCC_ERROR_STRING[] = {ENUM_GKCC_ERROR(ENUM_STRINGS)};

const char* gkcc_report_error(enum gkcc_error error, char* reporter);

#endif  // GKCC_MISC_H

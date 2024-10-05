# Copyright (C) 2023 Gary Kim <gary@garykim.dev>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https:#www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.20)
project(gkcc)

set(GKCC_UNDEFINED_BEHAVIOR_SANITIZER 1)

set (CMAKE_C_STANDARD 17)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -Werror-implicit-function-declaration -Wreturn-type -ggdb -fno-omit-frame-pointer -static-libubsan")
if(DEFINED GKCC_UNDEFINED_BEHAVIOR_SANITIZER)
       set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined -fsanitize=address")
endif()
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
set (GENERATED_DIR ${CMAKE_SOURCE_DIR}/generated)
set (INCLUDE_DIRS
        ${GENERATED_DIR}
        ${CMAKE_SOURCE_DIR}/src/lexical
        ${CMAKE_SOURCE_DIR}/src/parser
        ${CMAKE_SOURCE_DIR}/src/ast
        ${CMAKE_SOURCE_DIR}/src/scope
        ${CMAKE_SOURCE_DIR}/src/misc
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/3rdparty
        )

add_custom_command(
        OUTPUT
                ${GENERATED_DIR}/c.tab.c
                ${GENERATED_DIR}/c.tab.h
                ${GENERATED_DIR}/report.html
        COMMAND bison
        ARGS c.y --debug --output=${GENERATED_DIR}/c.tab.c --header=${GENERATED_DIR}/c.tab.h --html=${GENERATED_DIR}/report.html --report=lookaheads -v -Wall
        DEPENDS ${CMAKE_SOURCE_DIR}/src/parser/c.y
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/parser
        )

add_custom_command(
        OUTPUT
                ${GENERATED_DIR}/lex.yy.c
                ${GENERATED_DIR}/lex.yy.h
        COMMAND flex
        ARGS --header-file=${GENERATED_DIR}/lex.yy.h --outfile=${GENERATED_DIR}/lex.yy.c ${CMAKE_SOURCE_DIR}/src/lexical/c.lex
        DEPENDS ${CMAKE_SOURCE_DIR}/src/lexical/c.lex ${CMAKE_SOURCE_DIR}/src/lexical/lex_extras.h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/lexical
)

add_executable(lextester
        ${CMAKE_SOURCE_DIR}/src/lexical/test.c
        ${GENERATED_DIR}/lex.yy.c
        ${GENERATED_DIR}/c.tab.h
        ${CMAKE_SOURCE_DIR}/src/misc/misc.c
        ${CMAKE_SOURCE_DIR}/3rdparty/dmezh/backtrace.c
        )

target_include_directories(lextester PRIVATE ${INCLUDE_DIRS})
if(DEFINED GKCC_UNDEFINED_BEHAVIOR_SANITIZER)
        target_compile_definitions(lextester PRIVATE GKCC_UNDEFINED_BEHAVIOR_SANITIZER=1)
endif()

add_executable(parsetester
        ${CMAKE_SOURCE_DIR}/src/parser/parsetester.c
        ${GENERATED_DIR}/lex.yy.c
        ${GENERATED_DIR}/c.tab.c
        ${CMAKE_SOURCE_DIR}/src/ast/ast.c
        ${CMAKE_SOURCE_DIR}/src/scope/scope.c
        ${CMAKE_SOURCE_DIR}/src/misc/misc.c
        ${CMAKE_SOURCE_DIR}/src/ast/types.c
        ${CMAKE_SOURCE_DIR}/3rdparty/dmezh/backtrace.c
        ${CMAKE_SOURCE_DIR}/src/ast/ast_constructors.c
        ${CMAKE_SOURCE_DIR}/src/ast/ast_constructors.h
        ${CMAKE_SOURCE_DIR}/src/scope/scope_helpers.c
        ${CMAKE_SOURCE_DIR}/src/scope/scope_helpers.h
        )

target_include_directories(parsetester PRIVATE ${INCLUDE_DIRS})
if(DEFINED GKCC_UNDEFINED_BEHAVIOR_SANITIZER)
        target_compile_definitions(parsetester PRIVATE GKCC_UNDEFINED_BEHAVIOR_SANITIZER=1)
endif()

add_executable(gkcc_int
        ${CMAKE_SOURCE_DIR}/src/gkcc_int.c
        ${GENERATED_DIR}/lex.yy.c
        ${GENERATED_DIR}/c.tab.c
        ${CMAKE_SOURCE_DIR}/src/ast/ast.c
        ${CMAKE_SOURCE_DIR}/src/scope/scope.c
        ${CMAKE_SOURCE_DIR}/src/misc/misc.c
        ${CMAKE_SOURCE_DIR}/src/ast/types.c
        ${CMAKE_SOURCE_DIR}/3rdparty/dmezh/backtrace.c
        ${CMAKE_SOURCE_DIR}/src/ast/ast_constructors.c
        ${CMAKE_SOURCE_DIR}/src/ast/ast_constructors.h
        ${CMAKE_SOURCE_DIR}/src/scope/scope_helpers.c
        ${CMAKE_SOURCE_DIR}/src/scope/scope_helpers.h
        ${CMAKE_SOURCE_DIR}/src/ir/quads.c
        ${CMAKE_SOURCE_DIR}/src/ir/quads.h
        ${CMAKE_SOURCE_DIR}/src/ir/basic_block.c
        ${CMAKE_SOURCE_DIR}/src/ir/basic_block.h
        ${CMAKE_SOURCE_DIR}/src/ir/translators.c
        ${CMAKE_SOURCE_DIR}/src/ir/translators.h
        ${CMAKE_SOURCE_DIR}/src/ir/ir_full.c
        ${CMAKE_SOURCE_DIR}/src/ir/ir_full.h
        ${CMAKE_SOURCE_DIR}/src/target_code/x86.c
        ${CMAKE_SOURCE_DIR}/src/target_code/x86.c
        ${CMAKE_SOURCE_DIR}/src/target_code/x86.h
        ${CMAKE_SOURCE_DIR}/src/target_code/x86_inst.c
        ${CMAKE_SOURCE_DIR}/src/target_code/x86_inst.h
        )

target_include_directories(gkcc_int PRIVATE ${INCLUDE_DIRS})
if(DEFINED GKCC_UNDEFINED_BEHAVIOR_SANITIZER)
    target_compile_definitions(gkcc_int PRIVATE GKCC_UNDEFINED_BEHAVIOR_SANITIZER=1)
endif()

configure_file(${CMAKE_SOURCE_DIR}/src/gkcc gkcc COPYONLY)


cmake_minimum_required(VERSION 3.20)
project(gkcc)

set (CMAKE_C_STANDARD 17)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -ggdb")
set (GENERATED_DIR ${CMAKE_SOURCE_DIR}/generated)
set (INCLUDE_DIRS
        ${GENERATED_DIR}
        ${CMAKE_SOURCE_DIR}/src/lexical
        ${CMAKE_SOURCE_DIR}/src/parser
        ${CMAKE_SOURCE_DIR}/src/ast
        )

add_custom_command(
        OUTPUT
                ${GENERATED_DIR}/c.tab.c
                ${GENERATED_DIR}/c.tab.h
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

add_executable(parsetester
        ${CMAKE_SOURCE_DIR}/src/parser/parsetester.c
        ${GENERATED_DIR}/lex.yy.c
        ${GENERATED_DIR}/c.tab.c
        ${CMAKE_SOURCE_DIR}/src/ast/ast.c
        )

target_include_directories(parsetester PRIVATE ${INCLUDE_DIRS})

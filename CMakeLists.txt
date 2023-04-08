cmake_minimum_required(VERSION 3.20)
project(gkcc)

set (CMAKE_C_STANDARD 17)
set (INCLUDE_DIRS ./src/parser ./src/lexical ./src/ast)

add_custom_command(
        OUTPUT
                ${CMAKE_SOURCE_DIR}/src/parser/c.tab.c
                ${CMAKE_SOURCE_DIR}/src/parser/c.tab.h
        COMMAND bison
        ARGS c.y --debug --output=/c.tab.c --header=c.tab.h --html=report.html --report=lookaheads -v -Wall
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/parser
        )

add_custom_command(
        OUTPUT
                ${CMAKE_SOURCE_DIR}/src/lexical/lex.yy.c
                ${CMAKE_SOURCE_DIR}/src/lexical/lex.yy.h
        COMMAND flex
        ARGS --header-file=lex.yy.h c.lex
        DEPENDS ${CMAKE_SOURCE_DIR}/src/lexical/c.lex ${CMAKE_SOURCE_DIR}/src/lexical/lex_extras.h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/lexical
)

add_executable(parsetester
        ${CMAKE_SOURCE_DIR}/src/parser/parsetester.c
        ${CMAKE_SOURCE_DIR}/src/lexical/lex.yy.c
        ${CMAKE_SOURCE_DIR}/src/parser/c.tab.c
        ${CMAKE_SOURCE_DIR}/src/ast/ast.c
        )

target_include_directories(parsetester PRIVATE ${INCLUDE_DIRS})

cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

project(antrl4-demo)

set(SOURCES main.cpp)

set(ANTLR4_JAR_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/3rd/antlr/antlr-4.11.1-complete.jar")

find_package(antlr4-generator REQUIRED)
find_package(antlr4-runtime REQUIRED)

antlr4_generate( 
    antlrcpptest_lexer
    ${CMAKE_CURRENT_SOURCE_DIR}/ArithmeticLexer.g4
    LEXER
    FALSE
    FALSE
    "antlrcpptest"
)

antlr4_generate(
    antlrcpptest_parser
    ${CMAKE_CURRENT_SOURCE_DIR}/ArithmeticParser.g4
    PARSER
    TRUE
    FALSE
    "antlrcpptest"
    "${ANTLR4_TOKEN_FILES_antlrcpptest_lexer}"
    "${ANTLR4_TOKEN_DIRECTORY_antlrcpptest_lexer}"
)

message("ANTLR4_INCLUDE_DIR_antlrcpptest_parser: ${ANTLR4_INCLUDE_DIR_antlrcpptest_parser}")
message("ANTLR4_SRC_FILES_antlrcpptest_parser: ${ANTLR4_SRC_FILES_antlrcpptest_parser}")

include_directories(${ANTLR4_INCLUDE_DIR} ${ANTLR4_INCLUDE_DIR_antlrcpptest_lexer} ${ANTLR4_INCLUDE_DIR_antlrcpptest_parser})
add_executable(${PROJECT_NAME} ${SOURCES} ${ANTLR4_SRC_FILES_antlrcpptest_lexer} ${ANTLR4_SRC_FILES_antlrcpptest_parser})
add_dependencies(${PROJECT_NAME} antlr4_static)
target_link_libraries(${PROJECT_NAME} PRIVATE antlr4_static)

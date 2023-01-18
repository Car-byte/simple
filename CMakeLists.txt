cmake_minimum_required(VERSION 3.24)
project(simple)

set(CMAKE_CXX_STANDARD 20)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

add_executable(simple main.cc frontend/utils/token_sliding_window.h frontend/lexer/tokens/token.h frontend/lexer/tokens/token_type.h frontend/lexer/tokens/keyword_token.h frontend/lexer/tokens/keyword_type.h frontend/lexer/tokens/literal_number_token.h frontend/lexer/tokens/operator_type.h frontend/lexer/tokens/operator_token.h frontend/lexer/tokens/punctuator_type.h frontend/lexer/tokens/punctuator_token.h frontend/lexer/lexer.cc frontend/lexer/tokens/identifier_token.h backend/code_generator.cpp frontend/parser/parser.cpp frontend/utils/token_sliding_window.cpp backend/program.h frontend/parser/ast/ast_node.h frontend/parser/types/type.h frontend/parser/types/type_kind.h frontend/parser/types/type_factory.h frontend/parser/types/type_int32.h frontend/parser/types/type_float32.h frontend/parser/types/type_void.h frontend/parser/ast/ast_function.h frontend/parser/ast/ast_function_params.h frontend/parser/ast/ast_function_call.h frontend/parser/ast/ast_literal_int32.h frontend/parser/ast/ast_literal_float32.h frontend/parser/ast/ast_binary_op.h frontend/parser/ast/ast_assignment.h frontend/parser/ast/ast_var_declaration.h frontend/parser/ast/ast_variable.h frontend/parser/types/type_code_gen_imp.cpp frontend/parser/ast/ast_code_gen_imp.cpp)

llvm_map_components_to_libnames(llvm_libs support core irreader passes)

target_link_libraries(simple ${llvm_libs})

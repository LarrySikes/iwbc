cmake_minimum_required(VERSION 3.10)
project(iwbasic_compiler C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIE")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pie")


find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)
add_definitions(${LLVM_DEFINITIONS})

add_executable(iwbc 
    src/main.c
    src/parser.c
    src/generator.c
    src/lexer.c
)

llvm_map_components_to_libnames(llvm_libs support core irreader bitwriter analysis target native)
target_link_libraries(iwbc ${llvm_libs} stdc++)

add_executable(lexer_tests
    test/lexer_test.c
    src/lexer.c
)

add_executable(lexer_example
    examples/lexer_example.c
    src/lexer.c
)

install(TARGETS iwbc lexer_tests lexer_example
        RUNTIME DESTINATION bin)


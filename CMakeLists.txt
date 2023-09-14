cmake_minimum_required(VERSION 3.5)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project (CompilerRinha)

# find_package(jsoncpp REQUIRED)

include_directories(CompilerRinha include)

add_executable(CompilerRinha
	include/ast.h
	include/term.h
	include/context.h
	include/operations.h
	src/main.cpp
	src/ast.cpp
	src/term.cpp
	src/operations.cpp
	)

target_link_libraries(CompilerRinha jsoncpp)

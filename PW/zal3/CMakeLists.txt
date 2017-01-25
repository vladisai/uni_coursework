cmake_minimum_required (VERSION 3.1)
project(circuit)

set(CMAKE_C_FLAGS "-Wall -g")

add_library(err err.c)
add_library(message message.c)
add_library(node node.c)
add_library(list list.c)
add_library(utility utility.c)

add_executable(circuit circuit.c)
target_link_libraries(circuit utility list node message err)

ADD_CUSTOM_COMMAND(
    TARGET circuit
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/README ${CMAKE_CURRENT_BINARY_DIR}/README
   )

install(TARGETS DESTINATION .)

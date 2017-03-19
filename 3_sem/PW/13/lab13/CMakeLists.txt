cmake_minimum_required (VERSION 3.1)
project (MIMUW-SHMSEM C)

#set(CMAKE_C_STANDARD ...)
set(CMAKE_C_FLAGS "-g -Wall")

# http://stackoverflow.com/questions/10555706/
macro (add_executable _name)
  # invoke built-in add_executable
  _add_executable(${ARGV})
  if (TARGET ${_name})
    target_link_libraries(${_name} err)
  endif()
endmacro()

add_library(err err.c)
add_executable(get_memory get_memory.c)
target_link_libraries(get_memory rt)

add_executable(send_message send_message.c)
target_link_libraries(send_message rt)

add_executable(unnamed_memory unnamed_memory.c)
target_link_libraries(unnamed_memory rt)

add_executable(send_message_protected send_message_protected.c)
target_link_libraries(send_message_protected rt pthread)

add_executable(unnamed_memory_protected unnamed_memory_protected.c)
target_link_libraries(unnamed_memory_protected rt pthread)

add_executable(buffer buffer.c)
target_link_libraries(buffer rt pthread)

install(TARGETS DESTINATION .)

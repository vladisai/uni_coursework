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

add_executable(buffer buffer.c)
target_link_libraries(buffer rt pthread)

install(TARGETS DESTINATION .)

cmake_minimum_required (VERSION 3.1)
project (MIMUW-PTHREADS C)

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

add_executable(readers-writers readers-writers.c)
target_link_libraries(readers-writers pthread)

install(TARGETS DESTINATION .)

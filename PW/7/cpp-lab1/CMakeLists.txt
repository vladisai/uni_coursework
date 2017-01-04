cmake_minimum_required (VERSION 3.1)
project (MIMUW-CPCPP-lab1 CXX)

find_package ( Threads REQUIRED )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -Wall")

# http://stackoverflow.com/questions/10555706/
macro (add_executable _name)
    # invoke built-in add_executable
    _add_executable(${ARGV})
    if (TARGET ${_name})
        target_link_libraries(${_name} ${CMAKE_THREAD_LIBS_INIT} )
    endif()
endmacro()

add_executable(thread thread.cpp)
add_executable(thread-join thread-join.cpp)
add_executable(thread-detached thread-detached.cpp)
add_executable(thread-raii thread-raii.cpp)
add_executable(args args.cpp)
add_executable(res res.cpp)
add_executable(promise promise.cpp)
add_executable(promise-exception promise-exception.cpp)
add_executable(grep-seq grep-seq.cpp)

install(TARGETS DESTINATION .)

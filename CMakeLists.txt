cmake_minimum_required(VERSION 3.10)

# set the project name
project(Life VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(Life life.cc support.cc)

list(APPEND EXTRA_LIBS sfml-graphics)
list(APPEND EXTRA_LIBS sfml-window)
list(APPEND EXTRA_LIBS sfml-system)
target_link_libraries(Life PUBLIC ${EXTRA_LIBS})

cmake_minimum_required(VERSION 3.5)

project(01_Trimesh_Tiling LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DSTXXL)

set (STXXL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../external/stxxl)
set (STXXL_LIB ${STXXL_DIR}/build/lib/libstxxl.a)

file(MAKE_DIRECTORY ${STXXL_DIR}/build)

add_custom_command(
  OUTPUT ${STXXL_LIB}
  COMMAND cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
  #DEPENDS ${SOURCE_FILES} /tmp/bin/create_foo_hh main.cpp
  WORKING_DIRECTORY ${STXXL_DIR}
)

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/../../external/stxxl/include)
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/../../external/stxxl/build/include)
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/../../external/tclap/include)
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/../../external/cereal/include)

add_executable(${PROJECT_NAME} main.cpp ${STXXL_LIB})

target_link_libraries(${PROJECT_NAME} ${STXXL_LIB})

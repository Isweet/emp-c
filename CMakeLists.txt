cmake_minimum_required (VERSION 3.22.1)
project (symphony)
set(NAME "symphony")

set(CMAKE_BUILD_TYPE Debug)

if (APPLE)
  set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
endif()

find_package(Boost REQUIRED system)
find_package(emp-tool REQUIRED)
find_package(emp-ot REQUIRED)
find_package(emp-sh2pc REQUIRED)
find_package(spdz REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "-march=native -O3")
set(CMAKE_CXX_FLAGS "-march=native -O3")

add_library(empc SHARED src/emp.cpp src/spdz.cpp)

target_link_libraries(
  empc
  ${EMP-TOOL_LIBRARY}
  spdz)

target_include_directories(
  empc PUBLIC
  ${EMP-TOOL_INCLUDE_DIR}
  ${SPDZ_INCLUDE_DIR}
  src)

add_executable(and examples/and.c)
target_link_libraries(and empc)
target_include_directories(and PUBLIC empc)

add_executable(add examples/add.c)
target_link_libraries(add empc)
target_include_directories(add PUBLIC empc)

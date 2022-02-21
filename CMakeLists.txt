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

add_library(symphony SHARED src/net/net.cpp src/emp/emp.cpp src/spdz/spdz.cpp)

target_link_libraries(
  symphony
  ${EMP-TOOL_LIBRARY}
  spdz)

target_include_directories(
  symphony PUBLIC
  ${EMP-TOOL_INCLUDE_DIR}
  ${SPDZ_INCLUDE_DIR}
  src)

add_executable(and examples/and.c)
target_link_libraries(and symphony)
target_include_directories(and PUBLIC symphony)

add_executable(add examples/add.c)
target_link_libraries(add symphony)
target_include_directories(add PUBLIC symphony)

add_executable(mult examples/mult.c)
target_link_libraries(mult symphony)
target_include_directories(mult PUBLIC symphony)

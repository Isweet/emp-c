cmake_minimum_required (VERSION 3.10)
project (empc)
set(NAME "empc")

set(CMAKE_BUILD_TYPE Debug)

if (APPLE)
  set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
endif()

find_package(emp-tool REQUIRED)
find_package(emp-ot REQUIRED)

set (CMAKE_CXX_STANDARD 20)
set(CMAKE_C_FLAGS "-march=native -O3")
set(CMAKE_CXX_FLAGS "-march=native -O3")

add_library(empc SHARED src/empc.cpp)

target_link_libraries(
  empc
  ${EMP-TOOL_LIBRARY}
  ${OPENSSL_LIBRARIES}
  ${Boost_LIBRARIES})

target_include_directories(
  empc PUBLIC
  ${EMP-TOOL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  src)

add_executable(and examples/and.c)
target_link_libraries(and empc)
target_include_directories(and PUBLIC empc)

add_executable(add examples/add.c)
target_link_libraries(add empc)
target_include_directories(add PUBLIC empc)

cmake_minimum_required(VERSION 3.14)
project(Test VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(TEST_BUILD_TESTS "Build unit tests" ON)

# Library: all sources under src/ except main.cpp
file(GLOB_RECURSE TEST_LIB_SOURCES CONFIGURE_DEPENDS src/*.cpp)
list(FILTER TEST_LIB_SOURCES EXCLUDE REGEX ".*/main\\.cpp$")

add_library(test_lib ${TEST_LIB_SOURCES})
target_include_directories(test_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Executable
add_executable(test_app src/main.cpp)
target_link_libraries(test_app PRIVATE test_lib)

if(MSVC)
  target_compile_options(test_lib PUBLIC /utf-8)
  target_compile_options(test_lib PRIVATE /W4 /WX)
  target_compile_options(test_app PRIVATE /W4 /WX)
else()
  target_compile_options(test_lib PRIVATE -Wall -Wextra -Wpedantic -Werror)
  target_compile_options(test_app PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Tests
if(TEST_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

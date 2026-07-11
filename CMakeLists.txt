cmake_minimum_required(VERSION 3.20)
project(treewidth CXX)

# Default to an optimized build when the generator is single-config.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Header-only: Boost provides the Fibonacci heap only; no library is linked.
find_package(Boost REQUIRED)

# The project's own header-only code, exposed as an INTERFACE target that both
# the executable and the tests consume.
add_library(treewidth_lib INTERFACE)
target_include_directories(treewidth_lib INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}"
  ${Boost_INCLUDE_DIRS})
target_compile_features(treewidth_lib INTERFACE cxx_std_17)
target_compile_options(treewidth_lib INTERFACE -Wall -Wextra)

# Optional AddressSanitizer + UndefinedBehaviorSanitizer build (used in CI).
option(TREEWIDTH_SANITIZE "Build with ASan/UBSan" OFF)
if(TREEWIDTH_SANITIZE)
  target_compile_options(treewidth_lib INTERFACE
    -fsanitize=address,undefined -fno-omit-frame-pointer)
  target_link_options(treewidth_lib INTERFACE -fsanitize=address,undefined)
endif()

add_executable(treedecomp main.cpp)
target_link_libraries(treedecomp PRIVATE treewidth_lib)

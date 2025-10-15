cmake_minimum_required(VERSION 3.10.0)
project(i18n VERSION 0.1.0 LANGUAGES C CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(
  include

  # Include eternal dependencies
  external/json/single_include
)

# Add test subdirectory
add_subdirectory(test)

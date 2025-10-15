cmake_minimum_required(VERSION 3.10.0)
project(i18nTest VERSION 0.1.0 LANGUAGES C CXX)

add_executable(i18nTest
  src/main.cpp
)

include_directories(
  ../include
)

# target_link_libraries(i18nTest PRIVATE i18n)
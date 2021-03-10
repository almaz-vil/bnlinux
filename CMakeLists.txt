cmake_minimum_required(VERSION 2.6)
project(bnlinux)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RELEASE)
endif()


file(GLOB SOURCES
    *.h
    *.cpp
)

add_executable(bnlinux ${SOURCES})


find_package(X11 REQUIRED)
link_directories(${X11_LIBRARIES})
include_directories(${X11_INCLUDE_DIR})
target_link_libraries(bnlinux ${X11_LIBRARIES})


FIND_LIBRARY(X11_XTest_LIB Xtst ${X11_LIB_SEARCH_PATH})


target_link_libraries(bnlinux ${X11_XTest_LIB})
add_compile_options(
    -Werror

    -Wall
    -Wextra
    -Wpedantic

    -Wcast-align
    -Wcast-qual
    -Wconversion
    -Wctor-dtor-privacy
    -Wenum-compare
    -Wfloat-equal
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    -Wsign-conversion
    -Wsign-promo
)
cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(TEST_DATAS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test-datas)

# Ensure gtest submodule has been updated
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/googletest/googletest/src")
  message(NOTICE "googletest source not found")
  message(NOTICE "initializing/updating the googletest submodule...")

  execute_process(
        COMMAND git submodule update --init googletest
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

project(gtestbox)

add_executable(${PROJECT_NAME}
    dynlib_tests.cpp
)

# hook and configure dependencies
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# link with gtest
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# copy test datas dir to output dir
file(COPY ${TEST_DATAS_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(gtestbox
  PRIVATE
  GTest::GTest
  faust2godot)

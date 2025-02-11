cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(vmfpp)
set(CMAKE_CXX_STANDARD 23)

option(VMFPP_BUILD_TESTS "Build tests for vmfpp" OFF)

list(APPEND ${PROJECT_NAME}_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/detail/StringUtils.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/Reader.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/VMF.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/detail/StringUtils.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Reader.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/VMF.cpp")
add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

if(VMFPP_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.12.1)
    FetchContent_MakeAvailable(googletest)
    enable_testing()

    set(VMFPP_TEST_VMF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/test/res/test.vmf")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/test/Config.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/test/Config.h")

    add_executable(${PROJECT_NAME}test
            "${CMAKE_CURRENT_SOURCE_DIR}/test/Config.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/test/Reader.cpp")
    target_link_libraries(${PROJECT_NAME}test PUBLIC ${PROJECT_NAME} gtest_main)

    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME}test)
endif()

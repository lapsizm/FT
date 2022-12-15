cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_STANDARD 17)
option(BUILD_TESTS "Build tests" ON)

include("cmake/HunterGate.cmake")

HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.314.tar.gz"
        SHA1 "95c47c92f68edb091b5d6d18924baabe02a6962a"
)
project(fileter)

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

add_library(${PROJECT_NAME} STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/sources/FileTerrorist.cpp)

add_executable(demo
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        )

target_include_directories(${PROJECT_NAME} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:include>"
        )

target_link_libraries(demo ${PROJECT_NAME})

if (BUILD_TESTS)
    add_executable(tests
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/tests.cpp
            )
    target_link_libraries(tests ${PROJECT_NAME} GTest::gtest_main)
    enable_testing()
    add_test(NAME unit_tests COMMAND tests)
endif ()
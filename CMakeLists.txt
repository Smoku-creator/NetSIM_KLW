cmake_minimum_required(VERSION 3.13)
project(implementation)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wconversion)

include_directories(
        include
)

set(SOURCE_FILES
        src/storage_types.cpp
        src/package.cpp
        src/helpers.cpp
        src/nodes.cpp
        src/factory.cpp
        )

add_executable(net ${SOURCE_FILES} main.cpp)

set(SOURCE_FILES_TESTS
        test/test_Factory.cpp
        test/test_factory_io.cpp
        test/test_nodes.cpp
        test/test_package.cpp
        test/test_reports.cpp
        test/test_simulate.cpp
        test/test_storage_types.cpp
        )

add_executable(net_test ${SOURCE_FILES} ${SOURCE_FILES_TESTS} test/main_gtest.cpp)

target_include_directories(net_test PUBLIC
        googletest-master/googlemock/include
        googletest-master/googletest/include
        mocks
        )

add_subdirectory(googletest-master)

target_link_libraries(net_test gmock)

#target_compile_definitions(net_test PUBLIC EXERCISE_ID=2)
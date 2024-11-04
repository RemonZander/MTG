include(CTest)
find_package(GTest REQUIRED)
include(GoogleTest)

project(MTG_test_motionController)

add_executable(MTG_test_motionController ../../src/MotionController.cpp ./motorDriver_tester.cpp ./test.cpp)
set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -std=c++23 -Wall -Wextra -Wpedantic -g3 -O0")
gtest_discover_tests(MTG_test_motionController)

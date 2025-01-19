project(MTG_test_ludogame_init)

include(CTest)
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(MTG_test_ludogame_init ./LudoGame.cpp ./MotionController.cpp ./PathFinding.cpp ./test.cpp ./LudoUserInput.cpp)
set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -std=c++23 -Wall -Wextra -Wpedantic -g3 -O0")
target_link_libraries(MTG_test_ludogame_init GTest::gtest_main)
gtest_discover_tests(MTG_test_ludogame_init)

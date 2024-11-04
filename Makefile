PWD := .

MAIN_SRC_DIR := $(PWD)/MTG_Main/src
MAIN_TEST_DIR := $(PWD)/MTG_Main/test
BUILD_DIR := $(PWD)/build

MAIN_MOTIONCONTROLLER_SRC_FILES := $(MAIN_SRC_DIR)/motionController.cpp $(MAIN_SRC_DIR)/motionController.hpp
MAIN_PATHFINDING_SRC_FILES := $(MAIN_SRC_DIR)/PathFinding.cpp $(MAIN_SRC_DIR)/PathFinding.hpp

prepare_linux: $(BUILD_DIR)/Makefile
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR)
	cmake ..

MTGMain_all_tests: MTGMain_test_MotionController

# build/MTG_Main/test/motionController/MTG_test_motionController
MTGMain_test_MotionController: $(MAIN_MOTIONCONTROLLER_SRC_FILES) $(MAIN_TEST_DIR)/motionController/motorDriver_tester.cpp $(MAIN_TEST_DIR)/motionController/test.cpp
	cd $(BUILD_DIR)
	make MTG_test_motionController

MTGMain_pathFinding_debugExport: $(MAIN_PATHFINDING_SRC_FILES) $(MAIN_TEST_DIR)/pathFinding/main.cpp
	cd $(BUILD_DIR)
	make MTG_test_motionController

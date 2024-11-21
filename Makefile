PWD := .

MAIN_SRC_DIR := $(PWD)/MTG_Main/src
MAIN_TEST_DIR := $(PWD)/MTG_Main/test
BUILD_DIR := $(PWD)/build
BIN_DIR := $(PWD)/bin

MAIN_MOTIONCONTROLLER_SRC_FILES := $(MAIN_SRC_DIR)/motionController.cpp $(MAIN_SRC_DIR)/motionController.hpp
MAIN_PATHFINDING_SRC_FILES := $(MAIN_SRC_DIR)/PathFinding.cpp $(MAIN_SRC_DIR)/PathFinding.hpp

prepare_linux:
	mkdir -p $(BUILD_DIR) $(BIN_DIR)
	cd $(BUILD_DIR) && cmake ..

MTGMain_all_tests: MTGMain_test_MotionController

# build/MTG_Main/test/motionController/MTG_test_motionController
MTGMain_test_MotionController: prepare_linux
	cd $(BUILD_DIR) && make MTG_test_motionController
	cp $(BUILD_DIR)/MTG_Main/test/motionController/MTG_test_motionController $(BIN_DIR)/MTG_test_motionController

MTGMain_pathFinding_debugExport: prepare_linux
	cd $(BUILD_DIR) && make MTG_test_motionController

all_tests: MTGMain_test_MotionController MTGMain_pathFinding_debugExport

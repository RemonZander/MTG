# mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
# MTG_DIR := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))
MTG_DIR := $(shell pwd)

CHIP := esp32s3

MAIN_DIR := $(MTG_DIR)/MTG_Main
MAIN_SRC_DIR := $(MAIN_DIR)/main
MAIN_LIB_DIR := $(MAIN_DIR)/lib
MAIN_TEST_DIR := $(MAIN_DIR)/test
BUILD_DIR := $(MTG_DIR)/build
BIN_DIR := $(MTG_DIR)/bin

MAIN_MOTIONCONTROLLER_SRC_FILES := $(MAIN_SRC_DIR)/motionController.cpp $(MAIN_SRC_DIR)/motionController.hpp
MAIN_PATHFINDING_SRC_FILES := $(MAIN_SRC_DIR)/PathFinding.cpp $(MAIN_SRC_DIR)/PathFinding.hpp

ESP_IDF_CMD := IDF_TOOLS_PATH="$(MAIN_LIB_DIR)/esp-idf_tools" . "$(MAIN_LIB_DIR)/esp-idf/export.sh" && esp-idf.py

fullclean:
	rm -r "$(MTG_DIR)/MTG_Main/lib/esp-idf"* "$(MTG_DIR)/bin" "$(MTG_DIR)/build" "$(MTG_DIR)/MTG_Main/build"

prepare_linux:
	mkdir -p "$(BUILD_DIR)" "$(BIN_DIR)"
	cd "$(BUILD_DIR)" && cmake ..

install_dependencies_ubuntu:
	apt-get update
	apt-get install -y git wget cmake flex bison gperf python3 python3-pip python3-venv ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0
	apt-get install -y make libgtest-dev

install_ESP_IDF_linux:
# download
	test -d "$(MAIN_LIB_DIR)/esp-idf" || git clone -b v5.3.1 --depth 1 --recursive https://github.com/espressif/esp-idf.git "$(MAIN_LIB_DIR)/esp-idf"

# install
	mkdir -p "$(MAIN_LIB_DIR)/esp-idf_tools"
	"$(MAIN_LIB_DIR)/esp-idf/install.sh" $(CHIP)

MTGMain_build: prepare_linux
	echo -e "#!/bin/bash\n# IDF_PATH=\"$(MAIN_LIB_DIR)/esp-idf\"\n. \"$(MAIN_LIB_DIR)/esp-idf/export.sh\"\nidf.py \$$*" \
		> "$(BUILD_DIR)/esp_idf.sh"
	cd MTG_Main && bash "$(BUILD_DIR)/esp_idf.sh" build
	cp "$(MAIN_DIR)/build/MTG_Main.bin" "$(BIN_DIR)/MTG_Main.bin"
	cp "$(MAIN_DIR)/build/bootloader/bootloader.bin" "$(BIN_DIR)/bootloader.bin"
	cp "$(MAIN_DIR)/build/partition_table/partition-table.bin" "$(BIN_DIR)/partition-table.bin"

MTGMain_build_full: install_dependencies_ubuntu install_ESP_IDF_linux MTGMain_build

all_tests: MTGMain_all_tests

MTGMain_all_tests: MTGMain_pathFinding_debugExport MTG_test_ludogame_init MTG_test_move_pawn MTG_test_ludogame_logic

ludogame_tests: MTG_test_ludogame_init MTG_test_ludogame_logic

MTG_test_ludogame_init: prepare_linux
	cd "$(BUILD_DIR)" && make MTG_test_ludogame_init
	cp "$(BUILD_DIR)/MTG_Main/test/LudoGame_init/MTG_test_ludogame_init" "$(BIN_DIR)/MTG_test_ludogame_init"

MTG_test_ludogame_logic: prepare_linux
	cd "$(BUILD_DIR)" && make MTG_test_ludogame_logic
	cp "$(BUILD_DIR)/MTG_Main/test/LudoGame_logic/MTG_test_ludogame_logic" "$(BIN_DIR)/MTG_test_ludogame_logic"

MTG_test_move_pawn: prepare_linux
	cd "$(BUILD_DIR)" && make MTG_test_move_pawn
	cp "$(BUILD_DIR)/MTG_Main/test/move_pawn/MTG_test_move_pawn" "$(BIN_DIR)/MTG_test_move_pawn"

MTGMain_pathFinding_debugExport: prepare_linux
	cd "$(BUILD_DIR)" && make MTG_pathfind_debugExport
	cp "$(BUILD_DIR)/MTG_Main/test/pathFinding/MTG_pathfind_debugExport" "$(BIN_DIR)/MTG_pathfind_debugExport"

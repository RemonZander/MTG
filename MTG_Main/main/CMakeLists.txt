
idf_component_register(
	SRCS
		"main.cpp"
		"GameBoard.cpp"
		"LudoGame.cpp"
		"PathFinding.cpp"
		"Player.cpp"
		"LudoBoard.cpp"
		"LudoUserInput.cpp"
		"Memory.cpp"
		"pawn.cpp"
		"UserInputModule.cpp"
		"MotionController.cpp"
		"motorDriver.idf.cpp"
   INCLUDE_DIRS .)
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++23)

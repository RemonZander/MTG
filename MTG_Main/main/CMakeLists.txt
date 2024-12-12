
idf_component_register(
	SRCS
		"main.cpp"
		"GameBase.cpp"
		"LudoGame.cpp"
		"PathFinding.cpp"
		"Player.cpp"
		"LudoUserInput.cpp"
		"Pawns.cpp"
		"UserInputModule.cpp"
		"MotionController.cpp"
		"motorDriver.idf.cpp"
   INCLUDE_DIRS .)
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++23)

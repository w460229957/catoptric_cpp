CXX = g++
CXXFLAGS = -Wall --std=c++11
SOURCE = 	/home/pi/Desktop/catoptric/catoptric_cpp/JNI-Interface/commands/JNI_MoveCommand.cpp	\
			/home/pi/Desktop/catoptric/catoptric_cpp/CatoptricController.cpp \
			/home/pi/Desktop/catoptric/catoptric_cpp/CatoptricSurface.cpp	\
			/home/pi/Desktop/catoptric/catoptric_cpp/CatoptricRow.cpp \
			/home/pi/Desktop/catoptric/catoptric_cpp/SerialFSM.cpp \
			/home/pi/Desktop/catoptric/catoptric_cpp/prep_serial.cpp 

JNI_MoveCommand_test:	$(SOURCE) JNI_MoveCommand_test.cpp
	$(CXX) $(CXXFLAGS) JNI_MoveCommand_test.cpp	\
		$(SOURCE) -o JNI_MoveCommand_test

JNI_QuitComand_test:	$(SOURCE)	JNI_QuitCommand_test.cpp
	$(CXX) $(CXXFLAGS) JNI_QuitCommand_test.cpp	\
		$(SOURCE) -o JNI_QuitComand_test

JNI_ResetCommand_test:	$(SOURCE) JNI_ResetCommand_test.cpp
	$(CXX) $(CXXFLAGS) JNI_ResetCommand_test.cpp	\
		$(SOURCE) -o JNI_ResetCommand_test


JNI_TestCommand_test:	$(SOURCE)	JNI_TestCommand_test.cpp
	$(CXX) $(CXXFLAGS) JNI_TestCommand_test.cpp	\
		$(SOURCE) -o JNI_TestCommand_test

clear:
	rm -f JNI_ResetCommand_test	JNI_MoveCommand_test	JNI_QuitComand_test	\
		JNI_TestCommand_test

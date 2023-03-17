OUT_NAME 	= libcatoptric_cpp.so
CMPL_SRCS	= CatoptricSurface.cpp CatoptricRow.cpp prep_serial.cpp SerialFSM.cpp CatoptricController.cpp JNI_JNI.cpp \
			  JNI-Interface/service/JNI_service_impTest.cpp	\
			  JNI-Interface/commands/JNI_MoveCommand.cpp	JNI-Interface/commands/JNI_QuitCommand.cpp \
			  JNI-Interface/commands/JNI_ResetCommand.cpp	JNI-Interface/commands/JNI_TestCommand.cpp \

# CMPL_FLAGS	= -I/Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/JavaVM.framework/Versions/A/Headers
CMPL_FLAGS	= -I/usr/lib/jvm/java-1.11.0-openjdk-armhf/include -I/usr/lib/jvm/java-1.11.0-openjdk-armhf/include/linux

makecat: $(CMPL_SRCS)
	g++ -Wall --std=c++11 $(CMPL_SRCS) -shared -o $(OUT_NAME) $(CMPL_FLAGS)
clean:
	
standalone:
	g++ -Wall --std=c++11 CatoptricSurface.cpp CatoptricRow.cpp prep_serial.cpp SerialFSM.cpp CatoptricController.cpp CatoptricArrayMain.cpp -o catoptric_cpp

# g++ -Wall --std=c++11 CatoptricController.cpp CatoptricSurface.cpp CatoptricRow.cpp prep_serial.cpp SerialFSM.cpp CatoptricJNI.cpp -shared -o $(out_name) -I/Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/JavaVM.framework/Versions/A/Headers

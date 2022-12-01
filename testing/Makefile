OUT_NAME 	= libtesting.so
CMPL_SRCS	= testing_JNI.cpp
# CMPL_FLAGS	= -I/Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/JavaVM.framework/Versions/A/Headers
CMPL_FLAGS	= -I/usr/lib/jvm/java-1.11.0-openjdk-armhf/include -I/usr/lib/jvm/java-1.11.0-openjdk-armhf/include/linux
makecat: $(CMPL_SRCS)
	g++ -Wall --std=c++11 $(CMPL_SRCS) -shared -o $(OUT_NAME) $(CMPL_FLAGS)
clean:
	rm $(OUT_NAME) 2> /dev/null


# g++ -Wall --std=c++11 CatoptricController.cpp CatoptricSurface.cpp CatoptricRow.cpp prep_serial.cpp SerialFSM.cpp CatoptricJNI.cpp -shared -o $(out_name) -I/Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/JavaVM.framework/Versions/A/Headers

OUT_NAME 	= libcatoptric_cpp.so
WORKING_DIR	= source
CMPL_SRCS	= CatoptricSurface.cpp CatoptricRow.cpp \
			prep_serial.cpp SerialFSM.cpp CatoptricController.cpp JNI_JNI.cpp

# CMPL_FLAGS	= -I/Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/JavaVM.framework/Versions/A/Headers
CMPL_FLAGS	= -I/usr/lib/jvm/java-1.11.0-openjdk-armhf/include -I/usr/lib/jvm/java-1.11.0-openjdk-armhf/include/linux \
			  -I$(shell echo $(JAVA_HOME))/include \
			  -I$(shell find / -type d -exec test -e '{}'/jni_md.h \; -print)\
			  -lpthread 

makecat: 
	cd $(WORKING_DIR) && g++ -Wall -fPIC --std=c++11 $(CMPL_SRCS) -shared -o $(OUT_NAME) $(CMPL_FLAGS)
clean:
	rm -f $(OUT_NAME)

# g++ -Wall --std=c++11 CatoptricController.cpp CatoptricSurface.cpp CatoptricRow.cpp prep_serial.cpp SerialFSM.cpp CatoptricJNI.cpp -shared -o $(out_name) -I/Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/JavaVM.framework/Versions/A/Headers

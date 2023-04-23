OUT_NAME 	= libcatoptric_cpp.so
WORKING_DIR	= source
JAVA_HEADER_DIR = $(shell echo $(JAVA_HOME))
CMPL_SRCS	= CatoptricSurface.cpp CatoptricRow.cpp \
			Prep_serial.cpp SerialFSM.cpp ControllerNative.cpp

CMPL_FLAGS	= -I$(JAVA_HEADER_DIR)/include \
			  -I$(JAVA_HEADER_DIR)/include/linux\
			  -lpthread 

makecat: 
	cd $(WORKING_DIR) && g++-9 -Wall -fPIC --std=c++11 $(CMPL_SRCS) -shared -o $(OUT_NAME) $(CMPL_FLAGS)
clean:
	rm -f $(OUT_NAME)
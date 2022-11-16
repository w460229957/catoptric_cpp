out_name = catoptric_out.so

makecat: CatoptricSurface.cpp CatoptricRow.cpp prep_serial.cpp SerialFSM.cpp
	g++ -Wall --std=c++11 CatoptricController.cpp CatoptricSurface.cpp CatoptricRow.cpp prep_serial.cpp SerialFSM.cpp CatoptricJNI.cpp -shared -o $(out_name)

clean:
	rm $(out_name) 2> /dev/null

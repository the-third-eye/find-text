connected_components: conn_comp.o
	g++ -o connected_components conn_comp.o -lopencv_core \
	   	-lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui

conn_comp.o: conn_comp.cpp
	g++ -c conn_comp.cpp

clean:
	rm connected_components conn_comp.o

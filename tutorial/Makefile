mat : mat.cpp 
	g++ mat.cpp `pkg-config --cflags opencv` `pkg-config --libs opencv` -std=c++11 -o mat.out

mouse_event : mouse_event.cpp 
	g++ mouse_event.cpp `pkg-config --cflags opencv` `pkg-config --libs opencv` -std=c++11 -o mouse.out
clean:
	rm -r *.out

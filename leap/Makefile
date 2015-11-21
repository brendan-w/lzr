LEAP_LIBRARY := ./libLeap.so -Wl,-rpath,./
LZR_LIBRARY  := ../liblzr/liblzr.so -lzmq -lm -lpthread


Sample: Sample.cpp
	$(CXX) -Wall -g -I./include Sample.cpp -o Sample $(LEAP_LIBRARY) $(LZR_LIBRARY)

clean:
	rm -rf Sample 

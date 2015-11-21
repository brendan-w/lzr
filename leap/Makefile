LEAP_LIBRARY := ./libLeap.so -Wl,-rpath,./

Sample: Sample.cpp
	$(CXX) -Wall -g -I./include Sample.cpp -o Sample $(LEAP_LIBRARY)

clean:
	rm -rf Sample 

all: main.cpp
	c++ -std=c++11 -o renderer main.cpp -O3
clean:
	rm *.ppm renderer
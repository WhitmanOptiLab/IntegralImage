all: perftest

Makefile:

perftest: integralImage
	./integralImage

integralImage: integralImage.o
	g++ -O3 $^ -o $@ -fopenmp

%.o : %.cpp image.hpp Makefile
	g++ -c -o $@ $< -O3

.PHONY: clean
clean:
	rm -f ./*.o integralImage

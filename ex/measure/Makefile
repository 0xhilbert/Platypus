

all: libmeasure.a

libmeasure.a: measure.cpp measure.h
	g++ -c measure.cpp -o measure.o
	ar rcs libmeasure.a measure.o

clean:
	-rm measure.o libmeasure.a
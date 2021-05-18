

all: main

lib:
	make -C ../measure/

main: lib main.cpp
	g++ main.cpp -L../measure -I../measure -l:libmeasure.a -o main -lpthread -lrt -Os

run: main
	sudo taskset -c 1 ./main 2> log.csv

clean:
	make -C ../measure/ clean
	rm -r main
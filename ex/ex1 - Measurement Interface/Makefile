.SILENT:

all: main

lib:
	make -C ../measure/

main: lib main.cpp
	g++ main.cpp -L../measure -I../measure -l:libmeasure.a -o main

run: main
	sudo taskset -c 1 ./main

clean:
	make -C ../measure/ clean
	rm -r main
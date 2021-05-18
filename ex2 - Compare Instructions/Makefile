.SILENT:

all: main

lib:
	make -C ../measure/

main: lib main.cpp
	g++ main.cpp -L../measure -I../measure -l:libmeasure.a -o main

run: main
	sudo taskset -c 1 ./main imul 2> imul.csv
	sudo taskset -c 1 ./main add 2> add.csv
	sudo taskset -c 1 ./main nop 2> nop.csv

plot:
	python3 plot.py imul.csv add.csv nop.csv

clean:
	make -C ../measure/ clean
	rm -r main imul.csv add.csv nop.csv
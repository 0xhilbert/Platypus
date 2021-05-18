
GROUND_TRUTH:=0x$(shell sudo grep startup_64 /proc/kallsyms | cut -d' ' -f1 | head -1 )

.SILENT:

all: main

lib:
	make -C ../measure/

main: lib main.cpp
	g++ main.cpp -L../measure -I../measure -l:libmeasure.a -o main -lpthread -lrt -O3

run: main
	echo $(GROUND_TRUTH)
	sudo taskset -c 1 ./main $(GROUND_TRUTH)
	
clean:
	make -C ../measure/ clean
	rm -r main
CC=clang

build:
	-mkdir out 
	$(CC) src/main.c -o out/main -lraylib -lm
	cp -r src/resources ./out/

run:
	cd out && ./main

all:
	make build
	make run


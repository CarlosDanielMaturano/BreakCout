CC=clang

build:
	$(CC) src/main.c -o out/main -lraylib

run:
	./out/main

all:
	make build
	make run


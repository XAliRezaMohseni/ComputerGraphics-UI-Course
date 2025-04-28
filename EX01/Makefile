

.PHONY: build, clean, run


build:
	clang -g --verbose -lglut -lGL -lX11 ./main.c -o main

run: build
	./main

clean:
	rm ./main

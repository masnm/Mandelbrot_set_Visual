CC=gcc
cflags=-O2 -std=c11 -Wshadow -Wall -pedantic -lm
dbg=-fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG -g -DLOCAL
glut=-lGL -lGLU -lglut -lGLEW
objs=obj/shader.o

bin/main.out: src/main.c $(objs)
	$(CC) $(cflags) $(objs) -g src/main.c -o bin/main.out $(glut)

obj/shader.o: include/shader.h src/shader.c
	$(CC) $(cflags) -c src/shader.c -o obj/shader.o $(glut)

run: bin/main.out
	bin/main.out -gldebug

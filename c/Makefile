all: main run

#Программу можно запускать с аргументами прямо из makefile
#Для этого вы можете прописать make ARGS="<аргументы>"
ARGS=

LINK= gcc -g -Wall -Wextra -Werror
COMPILE= gcc -c -g -Wall -Wextra -Werror

OUTPUT= bin/test

#Какие файлы программа использует
DEPENDENCIES=obj/main.o obj/config.o

main: ${DEPENDENCIES}
	${LINK} $^ -o ${OUTPUT}

obj/main.o: src/main.c src/config/config.c src/config/config.h
	${COMPILE} $< -o $@

obj/config.o: src/config/config.c src/config/config.h
	${COMPILE} $< -o $@

run:
	./${OUTPUT} ${ARGS}

memcheck:
	valgrind --tool=memcheck ${OUTPUT} ${ARGS}

init:
	mkdir bin
	mkdir obj
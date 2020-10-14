BIN = mandelbrot
CC = gcc
FLAGS = -Wall -pedantic -O2 -DNDEBUG
LIBS = -lGL -lglfw -lGLEW -lm
SRC = main.c GLutils.c

all:
	@echo
	@echo ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	@echo ~~~~~~~~~~~~~~~~~~~~~~~~~~~Building GNU/LINUX 64-bit~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	@echo ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	@echo
	${CC} ${FLAGS} -o ${BIN} ${SRC} ${LIBS}
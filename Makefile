all: mandelbrot

CC = nvcc

LDLIBS += -lGL -lX11 -lglut -lGLU
#CPPFLAGS += -arch=sm_20

#DEBUG = -g -Wall
OBJ = timer.o glut_window.o mandelbrot.o main.o

mandelbrot: $(OBJ)
	$(CC) $(LDLIBS) $(OBJ) -o mandelbrot

main.o: main.cpp
	$(CC) $(DEBUG) $(CPPFLAGS) -c main.cpp

timer.o: timer.cpp
	$(CC) $(DEBUG) $(CPPFLAGS) -c timer.cpp

glut_window.o: glut_window.cpp
	$(CC) $(DEBUG) $(CPPFLAGS) -c glut_window.cpp

mandelbrot.o: mandelbrot.cu
	$(CC) $(DEBUG) $(CPPFLAGS) -c mandelbrot.cu
 
clean: 
	rm -fv $(OBJ) mandelbrot


all: mandelbrot

CC = nvcc

LDLIBS += -lGL -lX11 -lglut -lGLU
CPPFLAGS += -O3 

OBJ = timer.o glut_window.o mandelbrot.o main.o

mandelbrot: $(OBJ)
	$(CC) $(LDLIBS) $(OBJ) -o mandelbrot

main.o: main.cpp
	$(CC) $(CPPFLAGS) -c main.cpp

timer.o: timer.cpp
	$(CC) $(CPPFLAGS) -c timer.cpp

glut_window.o: glut_window.cpp
	$(CC) $(CPPFLAGS) -c glut_window.cpp

mandelbrot.o: mandelbrot.cu
	$(CC) $(CPPFLAGS) -c mandelbrot.cu
 
clean: 
	rm -fv $(OBJ) mandelbrot


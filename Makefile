all:
	g++ -Wall -o graphics graphics1.cpp -lglut -lGLU -lGL
clean:
	rm graphics *.o

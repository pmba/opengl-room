linux:
	g++ main.cpp -o main -lglut -lGLU -lGL -lGLEW
	./main

windows:
	g++ main.cpp -o main -I"C:\MinGW\freeglut\include" -lopengl32 -L"F:\MinGW\freeglut\lib\x64" -lfreeglut -lopenGL32 -lglu32 -lglew32
	./main
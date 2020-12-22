linux:
	g++ main.cpp -o theRoom -lglut -lGLU -lGL -lGLEW
	./theRoom

windows:
	g++ main.cpp -o theRoom -I"C:\MinGW\freeglut\include" -lopengl32 -L"F:\MinGW\freeglut\lib\x64" -lfreeglut -lopenGL32 -lglu32 -lglew32
	./theRoom
linux:
	g++ main.cpp ./libs/src/*.cpp ./libs/src/utils.cpp -o theRoom -lglut -lGLU -lGL
	@echo "Done! Now run ./theRoom"

windows:
	g++ main.cpp -o theRoom -I"C:\MinGW\freeglut\include" -lopengl32 -L"F:\MinGW\freeglut\lib\x64" -lfreeglut -lopenGL32 -lglu32
	@echo "Done! Now run ./theRoom"
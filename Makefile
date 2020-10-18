all: build

build:
	g++ main.cpp ./libs/src/foundation.cpp ./libs/src/utils.cpp -o theRoom -lglut -lGLU -lGL
	@echo "Done! Now run ./theRoom"
# OpenGL Room

This is a simple C++ and OpenGL project to render a 3d room space with diverse objects.

<img src="./res/images/preview.png"
     alt="A simple room scene preview"
     style="float: left; margin-right: 10px;" />

## Prerequisites

The only prerequisites are:

- OpenGL, GLUT and GLEW
- C++ Compiler

## Usage

There are two ways of running the program

### Compiling and Executing (By Hand)

To compile the program just run the following command

```bash
g++ main.cpp -o main -lglut -lGLU -lGL -lGLEW
```

And once compiled, simply run the following command to execute it

```bash
./main
```

**P.S:** The above commands are for the g++ compiler

### Using Makefile

If you have all the prerequisites already installed, you can simply run the **make** command to compile and run. But you will need to have the Make installed as well

```
make
```

## Controls

### Movement Controls

| Command Key      | Action                                   | Description                                         |
| ---------------- | ---------------------------------------- | --------------------------------------------------- |
| **_W, A, S, D_** | Move Forwards, Left, Backwards and Right | `Capslock cannot be active`                         |
| **_Q, E_**       | Move Up and Down                         | `Capslock cannot be active`                         |
| **_Mouse_**      | Look Around                              | `Hold left click and move the mouse to look around` |

### Objects Controls

| Command Key | Action                         | Description                 |
| ----------- | ------------------------------ | --------------------------- |
| **_P_**     | Open and Close the door        | `Capslock cannot be active` |
| **_J_**     | Open and Close the window      | `Capslock cannot be active` |
| **_L_**     | Turn on and off the table lamp | `Capslock cannot be active` |
| **_V_**     | Turn on and off the fan        | `Capslock cannot be active` |

## Contributing

As this was a project made for study purposes only, pull requests are not welcome. But feel free to fork the repository and modify it as you wish.

## License

[MIT](./LICENSE)

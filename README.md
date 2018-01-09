# csm_anim

This is 3D-simulator of plane wave scattering by Charge Simulation Method (CSM).

![ss](https://user-images.githubusercontent.com/99647/34719168-08374164-f57d-11e7-8c71-81f6335bb25b.gif)


## Getting Started

### Prerequisites

This software depend on wxWidets and OpenGL/GLUT, so you need to install these.

#### For Mac

If you use Mac, OpenGL has already been installed.
You just need to install wxWidets.

```
brew install wxWidets
```

### Build and Execution

#### For Mac

```
# Clone this repository
$ git clone 

# Go into the repository
$ cd csm_anim

# Create Makefile and make
$ cmake .
$ make

# Execute Simulator
./bin/Main
```

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

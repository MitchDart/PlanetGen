This repository is currently used to build a simple OpenGL engine to render procedurally generated planets.

## Building

To configure and build the project, do the following from the root directory of the repository:

```bash
mkdir build
cd build
cmake ..
```

This will configure the CMake and will generate platform-specific build files. To build, execute the following from within the build directory:

```bash
cmake --build .
```

Or build using whatever generated build system CMake produced.


### GLAD

GLAD loader files will be generated based on the CMake cache variables set in [CMakeLists.txt](CMakeLists.txt). Setting these variables allows one to specify the parameters for the loader similar to what the corresponding [webservice](https://glad.dav1d.de/) allows you to do.

### GLFW

GLFW is linked automatically against the main project. This works without requiring any additional dependencies to be installed on Windows, but Linux may require some additional steps depending on your environment:

Make sure you have a working compiler (GCC / CLANG) and build system, and that they're accessible to CMake. Stable versions of GCC (note: this project supports C++20 by default).
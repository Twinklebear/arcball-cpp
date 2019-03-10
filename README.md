# Arcball Camera

A [Shoemake arcball camera](http://www.talisman.org/~erlkonig/misc/shoemake92-arcball.pdf)
in C++ which uses GLM for math. The camera
works in a right handed coordinate system for OpenGL. The
camera takes inputs from your application in normalized device
coordinates, where the top-left of the screen is [-1, 1] and
the bottom right is [1, -1].
To use the camera in your own code you can either build and install
it somewhere and find it via CMake, or just drop the files into your
project. In the latter case you'll need to set `-DGLM_ENABLE_EXPERIMENTAL`
in your compile flags. You can either get the camera transformation
matrix or the eye position, look and up vectors depending on your application.

An example which uses the camera to view a cube rendered with OpenGL
is included under example/. To build the example you'll also need SDL2
for window management.


CXX=g++
CFLAGS=-Wall -std=c++14 -ggdb

window.o: src/window/window.cpp
	$(CXX) $(CFLAGS) -c -Isrc/window src/window/window.cpp

camera.o: src/camera/camera.cpp
	$(CXX) $(CFLAGS) -c -Isrc/camera -Isrc/window src/camera/camera.cpp

shaderparam.o: src/shaderparam/shaderparam.cpp
	$(CXX) $(CFLAGS) -c -Isrc/shaderparam src/shaderparam/shaderparam.cpp

texture.o: src/texture/texture.cpp
	$(CXX) $(CFLAGS) -c -Isrc/texture src/texture/texture.cpp

shaderProgram.o: src/shaderProgram/shaderProgram.cpp
	$(CXX) $(CFLAGS) -c -Isrc/shaderProgram src/shaderProgram/shaderProgram.cpp

object.o: src/shaderparam/shaderparam.h src/object/object.cpp
	$(CXX) $(CFLAGS) -c -Isrc/object -Isrc/shaderProgram -Isrc/texture -Isrc/shaderparam src/object/object.cpp

render.o: src/render/render.cpp
	$(CXX) $(CFLAGS) -c \
-Isrc/render \
-Isrc/object \
-Isrc/texture \
-Isrc/shaderProgram \
-Isrc/shaderparam \
-Isrc/camera \
-Isrc/window \
src/render/render.cpp

build: render.o object.o shaderparam.o window.o camera.o texture.o shaderProgram.o src/main.cpp
	$(CXX) $(CFLAGS) \
-Isrc/render \
-Isrc/object \
-Isrc/texture \
-Isrc/shaderProgram \
-Isrc/shaderparam \
-Isrc/camera \
-Isrc/window \
-lGLFW -lGLEW -lSOIL -g -rdynamic -framework OpenGL \
render.o \
object.o \
window.o \
camera.o \
texture.o \
shaderparam.o \
shaderProgram.o \
src/main.cpp -o app

clean:
	rm -rf *.o app

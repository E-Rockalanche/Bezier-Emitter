CXX := g++
TARGET := assignment3
CFLAGS := -Wall -Wextra -std=c++11 -c -I./inc
LFLAGS := -lGL -lGLU -lglut -L/usr/lib64 -lstdc++ -lm
CLEAN := rm obj/*.o $(TARGET)

MAKE_OBJ = $(CXX) $< -o $@ $(CFLAGS)
MAKE_EXE = $(CXX) $^ -o $@ $(LFLAGS)

$(TARGET): obj/main.o obj/vec3.o obj/stb_image.o obj/path.o obj/lerp.o
	$(MAKE_EXE)
	
obj/main.o: src/main.cpp inc/vec3.hpp
	$(MAKE_OBJ)
	
obj/vec3.o: src/vec3.cpp inc/vec3.hpp
	$(MAKE_OBJ)
	
obj/path.o: src/path.cpp inc/path.hpp
	$(MAKE_OBJ)
	
obj/lerp.o: src/lerp.cpp inc/lerp.hpp
	$(MAKE_OBJ)

obj/stb_image.o: src/stb_image.c inc/stb_image.h
	$(MAKE_OBJ)

clean:
	$(CLEAN)
CXX := g++
TARGET := assignment3

ifeq ($(OS), windows)
	LFLAGS := -static -lm -lglut32cu -lglu32 -lopengl32
	CLEAN := del .\obj\*.o $(TARGET)
else
	LFLAGS := -lm -lGL -lGLU -lglut
	CLEAN := rm ./obj/*.o $(TARGET)
endif

CFLAGS := -Wall -Wextra -std=c++17 -c -I./inc

MAKE_OBJ = $(CXX) $< -o $@ $(CFLAGS)
MAKE_EXE = $(CXX) $^ -o $@ $(LFLAGS)

$(TARGET): obj/main.o obj/vec3.o obj/stb_image.o obj/path.o obj/lerp.o \
	obj/emitter.o obj/matrix.o load_texture.o parse_path.o
	$(MAKE_EXE)
	
obj/main.o: src/main.cpp inc/vec3.hpp
	$(MAKE_OBJ)
	
obj/emitter.o: src/emitter.cpp inc/emitter.hpp
	$(MAKE_OBJ)
	
obj/vec3.o: src/vec3.cpp inc/vec3.hpp
	$(MAKE_OBJ)
	
obj/path.o: src/path.cpp inc/path.hpp
	$(MAKE_OBJ)
	
obj/lerp.o: src/lerp.cpp inc/lerp.hpp
	$(MAKE_OBJ)
	
obj/matrix.o: src/matrix.cpp inc/matrix.hpp
	$(MAKE_OBJ)

obj/stb_image.o: src/stb_image.c inc/stb_image.h
	$(MAKE_OBJ)

obj/load_texture.o: src/load_texture.cpp inc/load_texture.hpp
	$(MAKE_OBJ)

obj/parse_path.o: src/parse_path.cpp inc/parse_path.hpp
	$(MAKE_OBJ)

clean:
	$(CLEAN)
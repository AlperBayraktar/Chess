# ====================================
# Change these according to your SDL2 setup

# Folder that contains SDL.h
SDL2_INCLUDE_PATH = C:\cpp\SDL2-2.0.16\x86_64-w64-mingw32\include\SDL2
# Folder that contains SDL_image.h
SDL2_IMAGE_INCLUDE_PATH = C:\cpp\SDL2_image-2.0.5\x86_64-w64-mingw32\include\SDL2

# Folders that contains lib files
SDL2_LIBRARY_PATH = C:\cpp\SDL2-2.0.16\x86_64-w64-mingw32\lib
SDL2_IMAGE_LIBRARY_PATH = C:\cpp\SDL2_image-2.0.5\x86_64-w64-mingw32\lib

# Note: .dll files comes with SDL2 and SDL2_image must be copied to build/executable folder
# ====================================

COMPILE_CPP = g++ -c ../src/*.cpp -O3 -Wall -m64 -I include -I $(SDL2_INCLUDE_PATH) -I $(SDL2_IMAGE_INCLUDE_PATH)
MOVE_OBJS = move *.o objects
OBJ_TO_EXE = objects/*.o -o executable/build -s -L $(SDL2_LIBRARY_PATH) -L $(SDL2_IMAGE_LIBRARY_PATH) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
EXE_PATH = ./executable/build.exe

Compile:
	$(COMPILE_CPP)
	$(MOVE_OBJS)
	g++ $(OBJ_TO_EXE)
	$(EXE_PATH)

Run:
	$(EXE_PATH)

DebugCompile:
	$(COMPILE_CPP)
	$(MOVE_OBJS)
	g++ -g $(OBJ_TO_EXE)
	gdb $(EXE_PATH)

DebugRun:
	gdb $(EXE_PATH)
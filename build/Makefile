# ====================================
# Change these variables according to instructions

# Folder that contains SDL.h
SDL2_INCLUDE_PATH = C:\cpp\SDL2-2.0.16\x86_64-w64-mingw32\include\SDL2
# Folder that contains SDL_image.h
SDL2_IMAGE_INCLUDE_PATH = C:\cpp\SDL2_image-2.0.5\x86_64-w64-mingw32\include\SDL2
# Folder that contains SDL_ttf.h
SDL2_TTF_INCLUDE_PATH = C:\cpp\SDL2_ttf-2.0.15\x86\include\SDL2
# Folder that contains SDL_GUI.h
SDL_GUI_INCLUDE_PATH = C:\Users\Lenovo\Desktop\programming\Projects\Chess\SDL_GUI\include

# Folders that contains lib files
SDL2_LIBRARY_PATH = C:\cpp\SDL2-2.0.16\x86_64-w64-mingw32\lib
SDL2_IMAGE_LIBRARY_PATH = C:\cpp\SDL2_image-2.0.5\x86_64-w64-mingw32\lib
SDL2_TTF_LIBRARY_PATH = C:\cpp\SDL2_ttf-2.0.15\x86\lib


# There two ways to build .exe
# 1- To build and run on cmd:
#make Build
#make Run

# 2- To build in cmd and run exe in file explorer:
# make Build

# When you build and run in cmd (1), BUILDING_FOR should be CMD
# When you build project in cmd and run .exe in file explorer (2), BUILDING_FOR should be CLICK_EXE

BUILDING_FOR = CMD

# Note: .dll files comes with SDL2 and SDL2_image must be copied to build/executable folder
# ====================================

# You dont need to touch these
COMPILE_CPP = g++ -c -D$(BUILDING_FOR) ../src/*.cpp -O3 -Wall -m64 -I include -I $(SDL_GUI_INCLUDE_PATH) -I $(SDL2_INCLUDE_PATH) -I $(SDL2_IMAGE_INCLUDE_PATH) -I $(SDL2_TTF_INCLUDE_PATH)
MOVE_OBJS = move *.o objects
OBJ_TO_EXE = objects/*.o -o executable/Chess -s -L $(SDL2_LIBRARY_PATH) -L $(SDL2_IMAGE_LIBRARY_PATH) -L $(SDL2_TTF_LIBRARY_PATH) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
EXE_PATH = ./executable/Chess.exe

Build:
	$(COMPILE_CPP)
	$(MOVE_OBJS)
	g++ $(OBJ_TO_EXE)

Run:
	$(EXE_PATH)

DebugBuild:
	$(COMPILE_CPP)
	$(MOVE_OBJS)
	g++ -g $(OBJ_TO_EXE)
	gdb $(EXE_PATH)

DebugRun:
	gdb $(EXE_PATH)
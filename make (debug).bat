@echo off
gcc src\main.c src\callbacks.c src\level.c src\io.c src\matrix.c src\models.c src\object.c src\other.c src\world.c -g -gdwarf-2 -Wall -Wextra -lglfw3 -lglew32 -lgdi32 -lopengl32 -o "bin\main.exe"
PAUSE
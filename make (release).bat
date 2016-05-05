@echo off
gcc src\main.c src\callbacks.c src\level.c src\io.c src\matrix.c src\models.c src\object.c src\other.c src\world.c -DNDEBUG -O3 -s -lglfw3 -lglew32 -lgdi32 -lopengl32 -o "bin\main(release).exe"
PAUSE
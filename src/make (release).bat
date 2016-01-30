@echo off
gcc main.c callbacks.c level.c io.c matrix.c models.c object.c other.c world.c -DNDEBUG -O3 -s -lglfw3 -lglew32 -lgdi32 -lopengl32 -o "bin\main(release).exe"
PAUSE
#include "raylib.h"

#define WIDTH 1200
#define HEIGHT 800

int main() {
    InitWindow(WIDTH, HEIGHT, "Julia Sets");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
        }
        EndDrawing();
    }
    CloseWindow();
}

#include "raylib.h"
#include <cmath>

constexpr auto WIDTH = 1920;
constexpr auto HEIGHT = 1080;


int main() {
    InitWindow(WIDTH, HEIGHT, "Julia Sets");

    Image image = GenImageColor(WIDTH, HEIGHT, WHITE);
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    Shader shader = LoadShader(0, "resources/le_shader.fs");

    int C_Loc = GetShaderLocation(shader, "C");
    int R_Loc{GetShaderLocation(shader, "R")};
    int max_iterations_Loc{GetShaderLocation(shader, "max_iterations")};

    Vector2 C{-0.8f,  0.156f};
    float R{2.0f};
    int max_iterations{100};

    // SetShaderValue(shader, C_Loc, &C, SHADER_UNIFORM_VEC2);
    SetShaderValue(shader, R_Loc, &R, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, max_iterations_Loc, &max_iterations, SHADER_UNIFORM_INT);

    SetTargetFPS(120);

    int dirX = 1;
    int dirY = 1;
    while (!WindowShouldClose()) {
        SetShaderValue(shader, C_Loc, &C, SHADER_UNIFORM_VEC2);

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginShaderMode(shader);
                DrawTexture(texture, 0, 0, WHITE);
            EndShaderMode();

        EndDrawing();
        
        C.x += dirX * 0.0002f;
        C.y += dirY * 0.0002f;
        if (std::abs(C.x) > 1.0f) dirX = -dirX;
        if (std::abs(C.y) > 0.3f) dirY = -dirY;
    }
    UnloadShader(shader);
    UnloadTexture(texture);

    CloseWindow();
}

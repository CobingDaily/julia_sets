#include "raylib.h"
#include <cmath>

constexpr auto WIDTH = 1280;
constexpr auto HEIGHT = 720;


int main() {
    InitWindow(WIDTH, HEIGHT, "Julia Sets");

    Image image = GenImageColor(WIDTH, HEIGHT, WHITE);
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    Shader shader = LoadShader(0, "resources/le_shader.fs");

    int C_Loc = GetShaderLocation(shader, "C");
    int R_Loc{GetShaderLocation(shader, "R")};
    int max_iterations_Loc{GetShaderLocation(shader, "max_iterations")};

    Vector2 C{0.0f, 0.0f};
    float R{2.0f};
    int max_iterations{300};

    // SetShaderValue(shader, C_Loc, &C, SHADER_UNIFORM_VEC2);
    SetShaderValue(shader, R_Loc, &R, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, max_iterations_Loc, &max_iterations, SHADER_UNIFORM_INT);

    SetTargetFPS(120);

    while (!WindowShouldClose())
    {
        float t = static_cast<float>(GetTime()) * 0.08f;
        C = { 0.7885f * std::cos(t), 0.7885f * std::sin(t) };
        SetShaderValue(shader, C_Loc, &C, SHADER_UNIFORM_VEC2);

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginShaderMode(shader);
                DrawTexture(texture, 0, 0, WHITE);
            EndShaderMode();

        EndDrawing();
    }
    UnloadShader(shader);
    UnloadTexture(texture);

    CloseWindow();
}

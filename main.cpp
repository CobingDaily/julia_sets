#include "raylib.h"
#include <cmath>

constexpr auto WIDTH = 800;
constexpr auto HEIGHT = 600;

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
    int max_iterations{1000};

    SetShaderValue(shader, R_Loc, &R, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, max_iterations_Loc, &max_iterations, SHADER_UNIFORM_INT);

    SetTargetFPS(120);

    while (!WindowShouldClose())
    {
        float t = static_cast<float>(GetTime()) * 0.2f;
        float k = 0.98f;
        C = {k * (0.5f * std::cos(t) - 0.25f * std::cos(2 * t)),
             k * (0.5f * std::sin(t) - 0.25f * std::sin(2 * t))};
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

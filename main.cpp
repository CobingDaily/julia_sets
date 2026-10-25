#include "raylib.h"
#include <cmath>
#include <cstdint>

#define MSF_GIF_IMPL
#include "msf_gif.h"

constexpr auto  GIF_FPS       = 50;
constexpr auto  GIF_SECONDS   = 10.0f;
constexpr auto  GIF_FRAMES    = static_cast<int>(GIF_FPS * GIF_SECONDS);
constexpr auto  GIF_DELAY_CS  = 100 / GIF_FPS;

constexpr auto WIDTH  = 1920;
constexpr auto HEIGHT = 1080;
#define TITLE "Julia Sets"

enum class Side {INNER, OUTER};
constexpr auto SIDE = Side::INNER;

int main() {
    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);
    InitWindow(WIDTH, HEIGHT, TITLE);
    ToggleBorderlessWindowed();

    auto image = GenImageColor(WIDTH, HEIGHT, WHITE);
    auto texture = LoadTextureFromImage(image);
    UnloadImage(image);

    auto shader = LoadShader(0, "resources/le_shader.fs");

    int C_Loc{GetShaderLocation(shader, "C")};
    int R_Loc{GetShaderLocation(shader, "R")};
    int max_iterations_Loc{GetShaderLocation(shader, "max_iterations")};

    Vector2 C{0.0f, 0.0f};
    float R{2.0f};
    int max_iterations{1000};

    SetShaderValue(shader, R_Loc, &R, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, max_iterations_Loc, &max_iterations, SHADER_UNIFORM_INT);

    float phase{0.0f};
    bool recording{false};
    int recFrame{0};
    MsfGifState gifState{};

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // ------------------------------------------
        // Start GIF recording on CTRL+R
        auto start_rec = [&recording, &recFrame, &gifState]() {
            if (!recording && IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_R)) {
                recording = true;
                recFrame = 0;
                int w = GetRenderWidth();
                int h = GetRenderHeight();
                msf_gif_begin(&gifState, w, h);
                SetWindowTitle(TITLE " [REC]");
            }
        }; start_rec();

        // ------------------------------------------
        // Update
        auto update = [&C, &shader, &C_Loc, &phase, &recording, &recFrame]() {
            if (recording) phase = 2.0f * PI * recFrame / GIF_FRAMES;
            else phase = static_cast<float>(GetTime()) * 0.2f;

            constexpr auto k = (SIDE == Side::OUTER) ? 1.7f : 0.99f;

            C = {k * (0.5f * std::cos(phase) - 0.25f * std::cos(2.0f * phase)),
                k * (0.5f * std::sin(phase) - 0.25f * std::sin(2.0f * phase))};
            SetShaderValue(shader, C_Loc, &C, SHADER_UNIFORM_VEC2);
        }; update();

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginShaderMode(shader);
                DrawTexture(texture, 0, 0, WHITE);
            EndShaderMode();

            // ------------------------------------------
            // Capture finished frame
            auto capture_frame = [&recording , &gifState, &recFrame]() {
                if (recording) {
                    auto frame = LoadImageFromScreen();
                    msf_gif_frame(&gifState,
                            static_cast<uint8_t*>(frame.data),
                            GIF_DELAY_CS,
                            16,
                            frame.width * 4);
                    UnloadImage(frame);

                    if (++recFrame >= GIF_FRAMES) {   // one full period done
                        MsfGifResult result = msf_gif_end(&gifState);
                        SaveFileData(TextFormat("%s/screenrecording.gif",
                                    GetApplicationDirectory()),
                                result.data,
                                static_cast<int>(result.dataSize));
                        msf_gif_free(result);
                        recording = false;
                        SetWindowTitle(TITLE);
                        TraceLog(LOG_INFO, "GIF saved (%d frames)", GIF_FRAMES);
                    }
                }
            }; capture_frame();
            
        EndDrawing();
    }
    UnloadShader(shader);
    UnloadTexture(texture);

    CloseWindow();
}

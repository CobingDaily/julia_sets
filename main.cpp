#include "raylib.h"
#include <algorithm>
#include <complex>

using namespace std::complex_literals;

constexpr auto WIDTH = 1200;
constexpr auto HEIGHT = 800;
constexpr auto R = 2;
constexpr auto C = -0.8 + 0.156i;
constexpr auto MAX_ITERS = 100;

std::complex<double> z_from_coord(const int& x, const int& y) {
    const double real = static_cast<double>(x) / WIDTH  * 2 * R - R;
    const double imag = static_cast<double>(y) / HEIGHT * 2 * R - R;
    return {real, imag};
}

std::complex<double> square(const std::complex<double>& z) {
    return z * z + C;
}

int count_iterations(std::complex<double>& z) {
    int iter = 0;
    
    while(std::abs(z) < R && iter < MAX_ITERS) {
        z = square(z);
        ++iter;
    }

    return iter;
}

Color iterations_to_color(const int& iterations) {
    if (iterations == MAX_ITERS) return BLACK;
    double t = std::sqrt(static_cast<double>(iterations) / MAX_ITERS);

    constexpr double TAU = 6.28318530718;
    const double d[3] = {0.00, 0.10, 0.20};

    auto ch = [&](int i) {
        double v = 0.5 + 0.5 * std::cos(TAU * (t + d[i]));
        return static_cast<unsigned char>(std::clamp(v, 0.0, 1.0) * 255.0);
    };

    return Color{ ch(0), ch(1), ch(2), 255 };
}

void plot_pixels() {
    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            auto z = z_from_coord(x, y);
            const auto iterations = count_iterations(z);
            const auto color = iterations_to_color(iterations);
            DrawRectangle(x, y, 1, 1, color);
        }
    }
}

int main() {
    InitWindow(WIDTH, HEIGHT, "Julia Sets");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            plot_pixels();

        }
        EndDrawing();
    }
    CloseWindow();
}

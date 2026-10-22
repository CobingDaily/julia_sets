#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Custom input
uniform vec2 C;
uniform float R;
uniform int max_iterations;

// Output fragment color
out vec4 finalColor;


vec2 mul_z(vec2 z1, vec2 z2) {
    // z1 = a + bi
    // z2 = c + di
    // => z1 z2 = ac + adi + bci - bd
    //          = (ac-bd) + (ad + bc)i
    float real = (z1.x * z2.x - z1.y * z2.y);
    float imag = (z1.x * z2.y + z1.y * z2.x);
    return vec2(real, imag);
}

vec2 square_z(vec2 z) {
    return mul_z(z, z);
}

vec2 do_iteration(vec2 z, vec2 C) {
    return mul_z(z, z) + C;
}

float magnitude(vec2 z) {
    return sqrt(z.x * z.x + z.y * z.y);
}

int count_iterations(vec2 z, vec2 C, int max_iterations) {
    int iter = 0;
    
    float mag = magnitude(z);
    while(mag < 2 && iter < max_iterations) {
        z = do_iteration(z, C);
        mag = magnitude(z);

        iter++;
    }

    return iter;
}

vec4 iterations_to_color(int iterations, int max_iterations) {
    if (iterations == max_iterations) return vec4(vec3(0.0), 1.0);
    float f_iterations = float(iterations);
    float f_max_iterations = float(max_iterations);

    float t = sqrt(f_iterations / f_max_iterations);
    vec3 d = vec3(0.0, 0.1, 0.2);


    float TAU = 6.28318530718;
    vec3 v = 0.5 + 0.5 * cos(TAU * (t + d));

    return vec4(v, 1.0);
}

void main()
{
    vec2 z = vec2(2.0 * R) * fragTexCoord - vec2(R);
    int iterations = count_iterations(z, C, max_iterations);
    vec4 color = iterations_to_color(iterations, max_iterations);

    finalColor = color * colDiffuse * fragColor;
}

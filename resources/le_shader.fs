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

vec2 do_iteration(vec2 z) {
    return mul_z(z, z) + C;
}

float count_smooth_iterations(vec2 z) {
    float iter = 0.0f;
    while(dot(z, z) < 256.0f * 256.0f && iter < max_iterations) {
        z = do_iteration(z);
        iter++;
    }

    if (iter == max_iterations) return -1.0f;

    return iter + 1 - log2(log2(length(z)));
}

vec4 smooth_iterations_to_color(float mu, vec2 z) {
    if (mu < 0.0) return vec4(vec3(0.0), 1.0);

    float t = mu * 0.04;
    vec3 d = vec3(0.0, 0.1, 0.2);
    float TAU = 6.28318530718;
    vec3 v = 0.5 + 0.5 * cos(TAU * (t + d));

    return vec4(v, 1.0);
}

void main()
{
    vec2 z = 2.0 * R * fragTexCoord - R;
    float mu = count_smooth_iterations(z);
    vec4 color = smooth_iterations_to_color(mu, z);

    finalColor = color * colDiffuse * fragColor;
}

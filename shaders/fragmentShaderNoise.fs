#version 330 core
out vec4 FragColor;
in vec3 position;

uniform float seed;
precision lowp    float;

// Gold Noise by dcerisano@standard3d.com
// https://www.shadertoy.com/view/ltB3zD
float PHI = 1.61803398874989484820459 * 00000.1; // Golden Ratio
float PI  = 3.14159265358979323846264 * 00000.1; // PI
float SQ2 = 1.41421356237309504880169 * 10000.0; // Square Root of Two

float gold_noise(vec2 coordinate, float seed){
    return fract(tan(distance(coordinate*(seed+PHI), vec2(PHI, PI)))*SQ2);
}

void main()
{
    float randColor = gold_noise(position.xy ,seed);
    FragColor = vec4(randColor, randColor, randColor, 1.0);
};
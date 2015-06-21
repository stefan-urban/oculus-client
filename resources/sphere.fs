#version 330

in vec2 vTexCoord;
in float test;

out vec4 vFragColor;

// color lookup table
vec4 colors[2] = vec4[2](
        vec4(0.1, 0.7, 0.3, 1.0),
        vec4(0.7, 0.3, 0.1, 1.0)
);

void main() {
    if (vTexCoord.x > 0.3 && vTexCoord.y < 0.6)
        vFragColor = colors[0];
    else
        vFragColor = colors[1];
}

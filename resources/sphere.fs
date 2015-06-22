#version 330

uniform float x_start[128];
uniform float y_start[128];

in vec3 vPolarCoord;

out vec4 vFragColor;

// color lookup table
vec4 colors[2] = vec4[2](
        vec4(0.1, 0.7, 0.3, 1.0),
        vec4(0.7, 0.3, 0.1, 1.0)
);

void main() {

    int x = 0, y = 0;

    int i;

    for (i = 0; i < 128; i++)
    {
        if (mod(vPolarCoord.y, 0.52) < x_start[i])
        {
            x = i;
            break;
        }
    }

    for (i = 0; i < 128; i++)
    {
        if (mod(vPolarCoord.z, 0.52) < y_start[i])
        {
            y = i;
            break;
        }
    }

    vec4 color = vec4(
        float(x) / 128,
        float(y) / 128,
        0.0f,
        1.0f
    );

    vFragColor = color;
}

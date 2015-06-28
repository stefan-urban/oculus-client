#version 330

#define PI 3.14159265

uniform mat4 Projection = mat4(1);
uniform mat4 ModelView = mat4(1);

uniform float fov_x = 60.0 * PI / 180.0;
uniform float fov_y = 60.0 * PI / 180.0;

in vec2 Position;
in float Color;

out float TexColor;

void main()
{
    // First transform the pixel position to spherical coordinates
    float azimuth = (Position.x - 0.5) * fov_x;
    float elevation = (Position.y - 0.5) * fov_y;

    // Convert these to cartesian coordinates
    vec4 Coordinates = vec4(
        cos(elevation) * cos(azimuth),
        cos(elevation) * sin(azimuth),
        sin(elevation),
        1.0
    );

    // Calc final position
    gl_Position = Projection * ModelView * Coordinates;
    gl_PointSize = 5.0;

    // Pass on color to fragment shader
    TexColor = Color;
}

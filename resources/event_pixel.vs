#version 330

#define PI 3.14159265

uniform mat4 Projection = mat4(1);
uniform mat4 ModelView = mat4(1);

uniform float FovX = 60.0 * PI / 180.0;
uniform float FovY = 60.0 * PI / 180.0;

uniform float ManAzimuth = 0.0 * PI / 180.0;
uniform float ManElevation = 20.0 * PI / 180.0;

in vec2 Position;
in float CameraId;
in float Color;

out float TexColor;

void main()
{
    // First transform the pixel position to spherical coordinates
    float azimuth = (Position.x - 0.5) * FovX;
    float elevation = (Position.y - 0.5) * FovY;

    // Translations for different camera
    azimuth += 60 * PI / 180 * CameraId;

    // Manual control with arrow keys
    azimuth += ManAzimuth;
    elevation += ManElevation;

    // Convert these to cartesian coordinates
    vec4 Coordinates = vec4(
        cos(elevation) * cos(azimuth),
        cos(elevation) * sin(azimuth),
        sin(elevation),
        1.0
    );

    // Calc final position
    gl_Position = Projection * ModelView * Coordinates;
    gl_PointSize = 4.0;

    TexColor = Color;
}

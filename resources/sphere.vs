#version 330

#define M_PI 3.1415926535897932384626433832795

uniform mat4 Projection = mat4(1);
uniform mat4 ModelView = mat4(1);

// Defaults are values from stored object file
uniform float fov_x_start = 0.0;
uniform float fov_x_end = 60.0;
uniform float fov_y_start = -30.0;
uniform float fov_y_end = 30.0;

in vec3 Position;
in float Color;

out vec2 pixelCoordinates;
out float texColor;

void main()
{
    gl_Position = Projection * ModelView * vec4(Position, 1);
    gl_PointSize = 10.0;

    // Convert to spherical
    float radius = sqrt(Position.x * Position.x + Position.y * Position.y + Position.z * Position.z);
    float azimuth = atan(Position.y, Position.x);
    float elevation = M_PI / 2 - acos(Position.z / radius);

    // Convert to radians
    azimuth *= 180.0 / M_PI;
    elevation *= 180.0 / M_PI;

    // Steps for a single elements
    float step_az = fov_x_end - fov_x_start;
    step_az /= 128.0;

    float step_ele = fov_y_end - fov_y_start;
    step_ele /= 128.0;

    // Calculate pixel coordinates from position
    float x = (azimuth - fov_x_start) / step_az;
    float y = (elevation - fov_y_start) / step_ele;

    pixelCoordinates = vec2(x, y);
    texColor = Color;
}

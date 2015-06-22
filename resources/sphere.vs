#version 330

uniform mat4 Projection = mat4(1);
uniform mat4 ModelView = mat4(1);

in vec3 Position;
in vec2 TexCoord;

out vec3 vPolarCoord;

void main() {
    gl_Position = Projection * ModelView * vec4(Position, 1);

    float radius = 1.0;
    float angle1 = 0.0, angle2 = 0.0;

    vPolarCoord = vec3(
        sqrt(Position.x * Position.x + Position.y * Position.y + Position.z * Position.z),
        atan(Position.y / Position.x),
        atan(sqrt(Position.x * Position.x + Position.y * Position.y) / Position.z)
    );
}

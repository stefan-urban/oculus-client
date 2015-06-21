#version 330

uniform mat4 Projection = mat4(1);
uniform mat4 ModelView = mat4(1);
uniform vec4 color = vec4(1);

in vec3 Position;
in vec2 TexCoord;

out vec4 vTexCoord;
out float test;

void main() {
    gl_Position = Projection * ModelView * vec4(Position, 1);

    vTexCoord = color;
}

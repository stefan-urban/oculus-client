#version 330

in float texColor;

out vec4 vFragColor;


void main()
{
    vFragColor = vec4(texColor, 0.0, 0.0, 1.0);
}

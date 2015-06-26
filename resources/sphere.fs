#version 330

in float texColor;

out vec4 vFragColor;


void main()
{
    if (texColor < 0)
        vFragColor = vec4(0.0, -1.0 * texColor, 0.0, 1.0);
    else
        vFragColor = vec4(texColor, 0.0, 0.0, 1.0);
}

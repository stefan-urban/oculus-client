#version 330

in float TexColor;

out vec4 vFragColor;


void main()
{
    if (TexColor < 0)
        vFragColor = vec4(0.0, -1.0 * TexColor, 0.0, 1.0);
    else
        vFragColor = vec4(0.0, TexColor, TexColor, 1.0);
}

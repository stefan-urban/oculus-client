#version 330

in float TexColor;
in float test;

out vec4 vFragColor;


void main()
{
    if (TexColor < 0)
        vFragColor = vec4(0.0, -1.0 * TexColor, test, 1.0);
    else
        vFragColor = vec4(TexColor, 0.0, test, 1.0);
}

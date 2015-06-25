#version 330

in vec2 pixelCoordinates;
in float texColor;

out vec4 vFragColor;


void main()
{
    int x = int(pixelCoordinates.x);
    int y = int(pixelCoordinates.y);


    vFragColor = vec4(texColor, 0.0, 0.0, 1.0);
}

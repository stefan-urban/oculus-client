#version 330


in vec2 pixelCoordinates;

out vec4 vFragColor;


void main()
{
    int x = int(pixelCoordinates.x);
    int y = int(pixelCoordinates.y);

    bool a = (mod(x, 2) == 0);
    bool b = (mod(y, 2) == 0);

    vec4 col1 = vec4(0.0,0.0,0.0,1.0);
    vec4 col2 = vec4(1.0,1.0,1.0,1.0);

    vFragColor = ((!a && b) || (a && !b)) ? col1 : col2;
}

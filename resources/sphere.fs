#version 330

uniform sampler2D sampler;
uniform vec3 intensity_map[512];

in vec2 pixelCoordinates;

out vec4 vFragColor;


void main()
{
    int x = int(pixelCoordinates.x);
    int y = int(pixelCoordinates.y);

    vec4 c = texture(sampler, pixelCoordinates);
    //c.w = 1.0;

    vFragColor = c;

    //vFragColor = vec4(0.0, 0.5, 0.0, 1.0);
}

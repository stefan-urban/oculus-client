#version 330

uniform vec3 intensity_map[512];

in vec2 pixelCoordinates;

out vec4 vFragColor;


void main()
{
    int x = int(pixelCoordinates.x);
    int y = int(pixelCoordinates.y);

    int i = 0;

    // Find them in the intensity_map
    for(i = 0; i < 512; i++)
    {
        if (intensity_map[i].x == -1.0)
        {
            break;
        }

        if ((intensity_map[i].x == float(x)) && (intensity_map[i].y == float(y)))
        {
            vFragColor = vec4(vec3(intensity_map[i].z), 1.0);
            return;
        }
    }

    //vFragColor = vec4(vec3(1.0 - float(i) / 500.0), 1.0);

    vFragColor = vec4(0.0, 0.5, 0.0, 1.0);
}

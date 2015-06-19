#version 330

uniform sampler2D sampler;
uniform float Alpha = 1.0;

in vec2 vTexCoord;
out vec4 vFragColor;

void main() {
    vec4 c = texture(sampler, vTexCoord);
    c.a = min(Alpha, c.a);
    vFragColor = c;
    //vFragColor = vec4(fract(vTexCoord), log(vTexCoord.x), 1.0);

    if (vTexCoord.x > 0.1 && vTexCoord.y < 0.6)
        vFragColor = vec4(1, 0, 0, 0);
    else
        vFragColor = vec4(0.1, 1, 0.5, 0);
}
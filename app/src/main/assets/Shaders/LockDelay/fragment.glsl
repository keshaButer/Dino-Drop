#version 300 es

precision mediump float;
out vec4 FragColor;

in vec2 Pos;
in vec2 TexCoord;

uniform vec4 uColor;
uniform float uProgress;

void main()
{
    float x = 1.0 - uProgress;
    float alpha = step(TexCoord.x, x);
    FragColor = vec4(uColor.rgb, alpha);
}

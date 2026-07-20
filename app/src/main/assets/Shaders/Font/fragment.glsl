#version 300 es

precision mediump float;
out vec4 FragColor;

in vec2 Pos;
in vec2 TexCoord;

uniform vec4 uColor;
uniform sampler2D uTexture;

void main()
{
    FragColor = vec4(uColor.rgb, uColor.a * texture(uTexture, TexCoord).r);
}
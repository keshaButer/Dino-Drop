#version 300 es

precision mediump float;

out vec4 FragColor;

in vec2 Pos;
in vec2 TexCoord;

uniform sampler2D uTexture;
uniform vec4 uColor;

void main()
{
    FragColor = texture(uTexture, TexCoord) * uColor;
}

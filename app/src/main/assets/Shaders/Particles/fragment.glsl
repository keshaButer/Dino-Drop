#version 300 es

precision mediump float;
out vec4 FragColor;

in vec2 Pos;
in vec2 TexCoord;

uniform float time;
uniform vec4 uColor;
uniform sampler2D uTexture;

void main()
{
    // vec4 texColor = texture(uTexture, TexCoord);

    FragColor = vec4(1.0);
}
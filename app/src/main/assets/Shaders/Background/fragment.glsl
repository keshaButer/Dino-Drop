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
    vec2 finalTexCoord = vec2(TexCoord.x + time, TexCoord.y - time);
    vec4 texColor = texture(uTexture, finalTexCoord);

    vec3 blendedColor = texColor.rgb * uColor.rgb;
    float blendedAlpha = texColor.a * uColor.a;

    FragColor = vec4(blendedColor.rgb, blendedAlpha);
}
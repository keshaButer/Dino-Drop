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
    vec2 finalTexCoord = TexCoord;
    vec4 texColor = texture(uTexture, finalTexCoord);

    vec4 colored = mix(texColor, vec4(1.0), 0.15);
    colored *= uColor;
    float colorAmount = 0.7; 
    vec3 finalRGB = mix(texColor.rgb, colored.rgb, colorAmount);
    FragColor = vec4(finalRGB, uColor.a);
}

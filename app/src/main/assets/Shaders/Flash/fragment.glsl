#version 300 es
precision mediump float;

out vec4 FragColor;
in vec2 TexCoord;

uniform vec4 uColor;
uniform float uProgress;

void main()
{
    vec3 finalRGB = uColor.rgb;
    float finalAlpha = smoothstep(1.0, 0.1, uProgress);

    float distToCenter = abs(TexCoord.x - 0.5);

    float wave = uProgress * 0.5;
    float alphaMultiplier = step(wave, distToCenter);

    finalAlpha *= alphaMultiplier;
    
    FragColor = vec4(finalRGB, finalAlpha);
}
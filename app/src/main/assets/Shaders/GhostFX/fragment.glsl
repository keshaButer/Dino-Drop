#version 300 es
precision mediump float;

out vec4 FragColor;
in vec2 TexCoord;

uniform vec4 uColor;
uniform float uProgress;
uniform int uWidthInCells;
uniform int uHeightInCells;

float rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    vec2 gridScale = vec2(7.5, 7.5) * 0.8;
    vec2 worldUV = vec2(TexCoord.x * float(uWidthInCells), TexCoord.y * float(uHeightInCells));
    vec2 uvGrid = worldUV * gridScale;

    vec2 cellId = floor(uvGrid);
    vec2 localUv = fract(uvGrid);

    float sizeFactor = clamp(1.3 - uProgress, 0.4, 1.0);
    float distToCenter = abs(localUv.x - 0.5) + abs(localUv.y - 0.5);
    float animatedDist = distToCenter / max(sizeFactor, 0.001);

    float rhombMask = smoothstep(0.45, 0.40, animatedDist);

    float individualOffset = rand(cellId) * 0.4;
    float particleAlpha = 1.0 - smoothstep(0.0, 1.0 - individualOffset, uProgress);

    float reverseProgress = 1.0 - uProgress;
    float fadeBottom = clamp(reverseProgress - 0.7, 0.0, 1.0);
    float fadeVolume = smoothstep(fadeBottom, reverseProgress, TexCoord.y);
    particleAlpha *= (1.0 - fadeVolume);

    float finalAlpha = rhombMask * particleAlpha;

    if (finalAlpha < 0.01) discard;

    vec3 gradientColor = mix(uColor.rgb, vec3(1.0, 1.0, 1.0), 1.0 - TexCoord.y - 0.3);
    vec3 finalColor = gradientColor;
    
    FragColor = vec4(finalColor, finalAlpha);
}
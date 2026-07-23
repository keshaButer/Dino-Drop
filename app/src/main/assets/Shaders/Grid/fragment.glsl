#version 300 es
precision mediump float;

out vec4 FragColor;
in vec2 FragPos;

uniform vec4 uColor;
uniform float uWaveCenterY;
uniform float uWaveRadius;
uniform float uWaveIntensity;

void main()
{
    vec4 finalColor = uColor;

    if (uWaveIntensity > 0.0)
    {
        float distY = abs(FragPos.y - uWaveCenterY);
        
        float waveFront = smoothstep(uWaveRadius - 0.08, uWaveRadius, distY) * 
                          smoothstep(uWaveRadius + 0.08, uWaveRadius, distY);
                          
        vec4 neonColor = vec4(0.0, 1.0, 1.0, 1.0);
        
        float flashFactor = waveFront * uWaveIntensity;
        finalColor = mix(finalColor, neonColor, flashFactor * 0.9);
    }

    FragColor = finalColor;
}

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
    vec4 texColor = texture(uTexture, TexCoord);

    // vec3 blendedColor = mix(texColor.rgb, uColor.rgb, 0.5);
    // float blendedAlpha = uColor.a * texColor.a;

    if (texColor.a < 0.01) discard;

    FragColor = vec4(texColor.rgb, uColor.a);
}

#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Output fragment color
out vec4 finalColor;

// Custom uniforms
uniform sampler2D texture0;
uniform vec2 resolution;
uniform float time;

void main()
{
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    
    // Эффект "жидкого" искажения
    float distortion = sin(uv.y * 10.0 + time) * 0.005;
    vec4 texel = texture(texture0, uv + distortion);
    
    // Матовое стекло (фрост)
    float noise = fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);
    vec4 glassColor = vec4(1.0, 1.0, 1.0, 0.15); // Белый налет
    
    finalColor = mix(texel, glassColor, 0.4) + noise * 0.02;
    finalColor.a = 0.8; // Полупрозрачность
}

#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec3 realColor;

uniform vec3 color;

void main()
{
    // diffuse 
    vec3 norm = normalize(Normal); //what direction is the surface facing?
    float diff = max(dot(norm, vec3(0.0f, 1.0f, 0.0f)), 0.0); //compare these two

    
    if (diff > 0.9f) {
        FragColor = vec4(realColor, 1.0);
    } else {
        FragColor = vec4(realColor / 2.0f, 1.0);
    }

    
    if (length(gl_FragCoord.xy - vec2(3840.0f / 2.0f, 2160.0f / 2.0f)) < 10.0f) {
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0);
    }
}
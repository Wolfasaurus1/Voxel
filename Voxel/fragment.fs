#version 450 core
out vec4 FragColor;

in vec3 normal;  
in vec4 color;
in vec3 FragPos;  
 
uniform vec3 lightPos;

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 objectColor = color.xyz;

    // ambient
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColor;
  	
    vec3 realLightPos = vec3(20, 30, 20);

    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(realLightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor; /// (distance(realLightPos, FragPos) * 0.05);
    FragColor = vec4(result, 1.0);
}
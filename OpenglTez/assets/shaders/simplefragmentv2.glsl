#version 330 core

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float smoothness;
};

struct Light{
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 o_normal;
in vec3 o_fragPos;
in vec2 o_texUV;

// uniform sampler2D texture1;
// uniform sampler2D texture2;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

void main()
{
    // FragColor = mix(texture(texture1, o_texUV),texture(texture2, o_texUV), 0.5);
    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 norm = normalize(o_normal);
    vec3 lightDir = normalize(light.pos - o_fragPos);
    float diff = max(dot(norm,lightDir), 0.0f);
    vec3 diffuse = light.diffuse * (diff*material.diffuse);

    // specular
    vec3 viewDir = normalize(viewPos - o_fragPos);
    vec3 reflectdir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectdir), 0.0), material.smoothness * 128);
    vec3 specular = light.specular * (spec * material.specular);

    FragColor = vec4(vec3(ambient+ diffuse + specular), 1.0f);
} 
#version 330 core

struct Material{
    vec4 ambient;
    vec4 diffuse;
    sampler2D diffuse_texture;
    vec4 specular;
    sampler2D specular_texture;
    float smoothness;
};

#define MAX_POINT_LIGHTS 20
struct PointLight{
    vec3 pos;

    float k0;
    float k1;
    float k2;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};
uniform PointLight uPointlight[MAX_POINT_LIGHTS];
uniform int countPointLights;

struct DirectionLight{
    vec3 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};
uniform DirectionLight uDirectionlight;


#define MAX_SPOT_LIGHTS 5
struct Spotlight{
    vec3 pos;
    vec3 direction;

    float k0;
    float k1;
    float k2;

    float cutoff;
    float outer_cutoff;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};
uniform Spotlight uSpotlight[MAX_SPOT_LIGHTS];
uniform int countSpotLights;

out vec4 FragColor;

in vec3 o_normal;
in vec3 o_fragPos;
in vec2 o_texUV;

// uniform sampler2D texture1;
// uniform sampler2D texture2;

uniform Material uMaterial;

uniform vec3 viewPos;

vec4 CalculatePointlight(int idx, vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap)
{
    /*
    // Sample the texture color
    vec3 diffTex = vec3(texture(uMaterial.diffuse_texture, o_texUV));
    vec3 specTex = vec3(texture(uMaterial.specular_texture, o_texUV));

    // Ambient
    vec3 ambient = uPointlight.ambient * (uMaterial.ambient);// * texColor);

    // Diffuse
    vec3 norm = normalize(o_normal);
    vec3 lightDir = normalize(uPointlight.pos - o_fragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = uPointlight.diffuse * diff * (uMaterial.diffuse * diffTex);

    // specular
    vec3 viewDir = normalize(viewPos - o_fragPos);
    vec3 reflectdir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectdir), 0.0), uMaterial.smoothness * 128);
    vec3 specular = uPointlight.specular * spec *(uMaterial.specular * specTex);
    */

    // Ambient
    vec4 ambient = uPointlight[idx].ambient * uMaterial.ambient;// * texColor);

    // Diffuse
    vec3 lightDir = normalize(uPointlight[idx].pos - o_fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = uPointlight[idx].diffuse * (diff * diffMap);

    // specular
    vec3 reflectdir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectdir), 0.0), uMaterial.smoothness * 128);
    vec4 specular = uPointlight[idx].specular * (spec * specMap);

    // this atten is to calculate the light impact based on distance 
    float dist = length(uPointlight[idx].pos - o_fragPos);
    float atten = 1.0 / (uPointlight[idx].k0 + uPointlight[idx].k1 * dist + uPointlight[idx].k2 * (dist * dist));

    return vec4(ambient + diffuse + specular)* atten;
}

vec4 CalculateDirectionlight(vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap)
{
    vec4 ambient = uDirectionlight.ambient * uMaterial.ambient;

    vec3 lightDir = normalize(-uDirectionlight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = uDirectionlight.diffuse* (diff * diffMap);

    vec3 reflectdir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectdir), 0.0), uMaterial.smoothness * 128);
    vec4 specular = uDirectionlight.specular * (spec * specMap);

    return ambient+ diffuse + specular;
}

vec4 CalculateSpotlight(int idx, vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap)
{
    vec3 lightDir = normalize(uSpotlight[idx].pos - o_fragPos);
    float theta = dot(lightDir, normalize(-uSpotlight[idx].direction));

    // ambient 
    vec4 ambient = uSpotlight[idx].ambient * uMaterial.ambient;
    // > cus using cosine not degree
    if(theta > uSpotlight[idx].cutoff)
    {
        // if in cutoff, render
        // diffuse 
        float diff = max(dot(norm, lightDir), 0.0);
        vec4 diffuse = uSpotlight[idx].diffuse * (diff * diffMap);

        // specular
        vec3 reflectdir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectdir),0.0), uMaterial.smoothness * 128);
        vec4 specular = uSpotlight[idx].specular * (spec * specMap);

        float intensity = (theta - uSpotlight[idx].outer_cutoff) / (uSpotlight[idx].cutoff - uSpotlight[idx].outer_cutoff);

        intensity = clamp(intensity, 0.0, 1.0);
        diffuse *= intensity;
        specular *= intensity;
        
        // this atten is to calculate the light impact based on distance 
        float dist = length(uSpotlight[idx].pos - o_fragPos);
        float atten = 1.0 / (uSpotlight[idx].k0 + uSpotlight[idx].k1 * dist + uSpotlight[idx].k2 * (dist * dist));
        return vec4(ambient + diffuse + specular) * atten;
    }

    return ambient;
}


uniform int noTex;

void main()
{
    vec3 norm = normalize(o_normal);
    vec3 viewDir = normalize(viewPos - o_fragPos);
    vec4 diffTex = texture(uMaterial.diffuse_texture, o_texUV);
    vec4 specTex = texture(uMaterial.specular_texture, o_texUV);
    vec4 diffMap = uMaterial.diffuse;
    vec4 specMap = uMaterial.specular;



    vec4 res = CalculateDirectionlight(norm, viewDir, diffMap, specMap);

    // point lights
    for(int i = 0; i < countPointLights; i++)
    {
        res+=  CalculatePointlight(i,norm, viewDir,diffMap, specMap) ;
    }

    // spot lights
    for(int i = 0; i < countSpotLights; i++)
    {
        res+=  CalculateSpotlight(i,norm, viewDir, diffMap, specMap) ;
    }

    FragColor = res;
} 
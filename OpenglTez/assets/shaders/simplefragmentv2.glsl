#version 330 core
out vec4 FragColor;

in vec2 o_texUV;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, o_texUV),texture(texture2, o_texUV), 0.5);
} 
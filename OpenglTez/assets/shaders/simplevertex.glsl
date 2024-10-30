#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texUV;
layout (location = 2) in vec3 color;

uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 o_color;
out vec2 o_texUV;

void main()
{
    gl_Position = uProjection * uView  * uTransform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    o_texUV = texUV;
    o_color = color;
}
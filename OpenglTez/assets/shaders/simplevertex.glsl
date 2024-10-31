#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
// layout (location = 3) in vec3 color;

uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uProjection;

// out vec3 o_color;
out vec2 o_texUV;
out vec3 o_normal;
out vec3 o_fragPos;

void main()
{
    o_fragPos = vec3(uTransform * vec4(aPos.x, aPos.y, aPos.z, 1.0));
    o_normal = mat3(transpose(inverse(uTransform))) * aNormal;

    gl_Position = uProjection * uView  * uTransform * vec4(o_fragPos, 1.0);
    o_texUV = aTexCoord;
    // o_color = color;
}
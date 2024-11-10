#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in ivec4 aBoneids;
layout (location = 4) in vec4 aWeights;
// layout (location = 3) in vec3 color;

uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uProjection;


// out vec3 o_color;
out vec2 o_texUV;
out vec3 o_normal;
out vec3 o_fragPos;
flat out ivec4 o_boneids;
out vec4 o_weights;

uniform bool hasBones;


const int MAX_BONES = 200;
uniform mat4 gBones[MAX_BONES];

void main()
{
    // mat4 boneTransform =  aWeights.x *gBones[aBoneids.x];
    // boneTransform +=  aWeights.y  *  gBones[aBoneids.y] ;
    // boneTransform +=  aWeights.z  *  gBones[aBoneids.z];
    // boneTransform +=  aWeights.w  *  gBones[aBoneids.w] ;


    vec4 transformedPos= vec4(0.0f);
    if(hasBones)
    {
      for(int i = 0 ; i < 4 ; i++)
      {
          if(aBoneids[i] == -1) 
              continue;
          if(aBoneids[i] >=MAX_BONES) 
          {
              transformedPos = vec4(aPos,1.0f);
              break;
          }
          vec4 localPosition = gBones[aBoneids[i]] * vec4(aPos,1.0f);
          transformedPos += localPosition * aWeights[i];
      }
     
     o_fragPos = vec3(uTransform * transformedPos);

    }
    else
    {
        transformedPos = vec4(aPos, 1.0);
        o_fragPos = vec3(uTransform * vec4(aPos.x, aPos.y, aPos.z, 1.0));
    }

    o_normal = mat3(transpose(inverse(uTransform))) * aNormal;

    gl_Position = uProjection * uView  * uTransform * transformedPos;
    o_texUV = aTexCoord;
    o_weights = aWeights;
    o_boneids = aBoneids;
    // o_color = color;
}
#pragma once
#ifndef Cube_hpp
#define Cube_hpp

#include "Model.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include<GLFW/glfw3.h>

class Cube : public Model
{
public:
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 size = glm::vec3(1.0f);

	Cube(glm::vec3 pos, glm::vec3 size):pos(pos), size(size)
	{

	}

	void Init(ShaderProgram shader)
	{
		int size_vertices = 36;

        float vertices[] = {
            // position          texcoord       normal              
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //   0.0f,  0.0f, -1.0f,    
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  //   0.0f,  0.0f, -1.0f,    
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  //   0.0f,  0.0f, -1.0f,    
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  //   0.0f,  0.0f, -1.0f,    
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  //   0.0f,  0.0f, -1.0f,    
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //   0.0f,  0.0f, -1.0f,    
                                    
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  //   0.0f,  0.0f,  1.0f,    
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  //   0.0f,  0.0f,  1.0f,    
             0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  //   0.0f,  0.0f,  1.0f,    
             0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  //   0.0f,  0.0f,  1.0f,    
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  //   0.0f,  0.0f,  1.0f,    
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  //   0.0f,  0.0f,  1.0f,    
                                   
            -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  //  -1.0f,  0.0f,  0.0f,    
            -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  //  -1.0f,  0.0f,  0.0f,    
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //  -1.0f,  0.0f,  0.0f,    
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //  -1.0f,  0.0f,  0.0f,    
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  //  -1.0f,  0.0f,  0.0f,    
            -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  //  -1.0f,  0.0f,  0.0f,    
                                   
             0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  //   1.0f,  0.0f,  0.0f,    
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  //   1.0f,  0.0f,  0.0f,    
             0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //   1.0f,  0.0f,  0.0f,    
             0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //   1.0f,  0.0f,  0.0f,    
             0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  //   1.0f,  0.0f,  0.0f,    
             0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  //   1.0f,  0.0f,  0.0f,    
                                    
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //   0.0f, -1.0f,  0.0f,    
             0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  //   0.0f, -1.0f,  0.0f,    
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  //   0.0f, -1.0f,  0.0f,    
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  //   0.0f, -1.0f,  0.0f,    
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  //   0.0f, -1.0f,  0.0f,    
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //   0.0f, -1.0f,  0.0f,    
                                   
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  //   0.0f,  1.0f,  0.0f,    
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  //   0.0f,  1.0f,  0.0f,    
             0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  //   0.0f,  1.0f,  0.0f,    
             0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  //   0.0f,  1.0f,  0.0f,    
            -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  //   0.0f,  1.0f,  0.0f,    
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f  //   0.0f,  1.0f,  0.0f,    
        };      

        std::vector<unsigned int> indices(size_vertices);
        for (int i = 0; i < 6; i++)
        {
            int startIndex = 4 * i;
            indices.push_back(startIndex + 2);
            indices.push_back(startIndex + 3);
            indices.push_back(startIndex);

            indices.push_back(startIndex);
            indices.push_back(startIndex + 2);
            indices.push_back(startIndex + 1);
        }
        Texture tex1("screenhider.png", "texture1"), tex2("tree-736885_640.png", "texture2");
        _meshes.push_back(Mesh(Vertex::GenList(vertices, size_vertices), indices, { tex1, tex2 }, &shader));
	}

    void Render(ShaderProgram* shader) {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, pos);
        transform = glm::scale(transform, size);
        //transform = glm::rotate(transform, float(glfwGetTime()) * glm::radians(-45.0f), glm::vec3(0.5f));
        shader->SetMat4("uTransform", transform);
        Model::Render(shader);
    }
};
#endif // !Cube_hpp

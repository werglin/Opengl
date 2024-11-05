#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<string>
#include<fstream>
#include<sstream>
#include<streambuf>

#include<glm/mat4x4.hpp>
#include<glm/gtc/matrix_transform.hpp>
//#include<glm/gtx/matrix_interpolation.hpp>

#include"ShaderProgram.hpp"
#include"Keyboard.hpp"
#include"Mouse.hpp"
#include"Joystick.hpp"
#include"Camera.hpp"
#include"Window.hpp"
#include"Texture.hpp"
#include"Models/Cube.hpp"
#include"Models/Lamp.hpp"
#include"Light.hpp"

Window window;

/*
* Triangle
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};
*/


float vertices[] = {
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

std::vector<unsigned int> indicesVec = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};


glm::mat4 _mouseTransform = glm::mat4(1.0f);
JoyStick mainJ(0);

Camera cameras[2] = { 
    Camera(glm::vec3(0.0f, 0.0f, 3.0f))
    , Camera(glm::vec3(10.0f, 10.0f, 13.0f)) 
};

int activeCam = 0;
float deltatime;
float lastframe;
bool flashLightOn = false;

void processInput(double dt)
{
    if (Keyboard::s_Key(GLFW_KEY_ESCAPE))
        window.SetShouldClose(true);

    if (Keyboard::s_KeyDown(GLFW_KEY_E))
    {
        flashLightOn = !flashLightOn;
    }

    // move cam
    if (Keyboard::s_Key(GLFW_KEY_W))
    {
        cameras[activeCam].UpdateCameraPos(CameraDirection::FORWARD, dt);
    }
    if (Keyboard::s_Key(GLFW_KEY_S))
    {
        cameras[activeCam].UpdateCameraPos(CameraDirection::BACKWARD, dt);
    }
    if (Keyboard::s_Key(GLFW_KEY_A))
    {
        cameras[activeCam].UpdateCameraPos(CameraDirection::LEFT, dt);
    }
    if (Keyboard::s_Key(GLFW_KEY_D))
    {
        cameras[activeCam].UpdateCameraPos(CameraDirection::RIGHT, dt);
    }
    if (Keyboard::s_Key(GLFW_KEY_LEFT_SHIFT))
    {
        cameras[activeCam].UpdateCameraPos(CameraDirection::DOWN, dt);
    }
    if (Keyboard::s_Key(GLFW_KEY_SPACE))
    {
        cameras[activeCam].UpdateCameraPos(CameraDirection::UP, dt);
    }

    if (Keyboard::s_KeyDown(GLFW_KEY_TAB))
    {
        activeCam += (activeCam == 0) ? 1 : -1;
    }
    
    double dx = Mouse::s_GetMouseDX(), dy = Mouse::s_GetMouseDY();
    if (dx != 0 || dy != 0)
    {
        cameras[activeCam].UpdateCameraDirection(dx * dt * 150.0f, dy * dt* 150.0f);
    }

    double scroolDy = Mouse::s_GetScrollDY();
    if (scroolDy != 0)
    {
        cameras[activeCam].UpdateCameraZoom(scroolDy);
    }


    mainJ.Update();
}

std::string loadShaderSrc(const char* filename)
{
    std::ifstream file;
    std::stringstream buf;

    std::string ret = "";
    file.open(filename);
    if (file.is_open())
    {
        buf << file.rdbuf();
        ret = buf.str();
    }
    else
    {
        std::cout << "Could not open " << filename<< std::endl;
    }

    file.close();
    return ret;
}

int main()
{


    if (!glfwInit())
        return -1;

    if (!window.Init())
    {
        std::cout << "Failed to create window" << std::endl;
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    window.SetParameters();


    ShaderProgram *sp1, *sp2, *shaderLamp;
    sp1 = new ShaderProgram();
    sp2 = new ShaderProgram();
    shaderLamp = new ShaderProgram();

    //sp1->AttachShader("assets/shaders/simplevertex.glsl", GL_VERTEX_SHADER);
    //sp1->AttachShader("assets/shaders/simplefragment.glsl", GL_FRAGMENT_SHADER);
    //sp1->Link();

    sp2->AttachShader("assets/shaders/simplevertex.glsl", GL_VERTEX_SHADER);
    sp2->AttachShader("assets/shaders/simplefragmentv2.glsl", GL_FRAGMENT_SHADER);
    sp2->Link();

    shaderLamp->AttachShader("assets/shaders/simplevertex.glsl", GL_VERTEX_SHADER);
    shaderLamp->AttachShader("assets/shaders/lamp_fs.glsl", GL_FRAGMENT_SHADER);
    shaderLamp->Link();
    // ------------------------------------
    // Texture tex1("screenhider.png", "texture1"), tex2("tree-736885_640.png", "texture2");
    // Model model;
    // model.Init();
    // model._meshes.push_back(Mesh(Vertex::GenList(vertices, 4), indicesVec, { tex1, tex2 }, sp2));


    glm::mat4 view = glm::mat4(1);
    glm::mat4 projection = glm::mat4(1);

    view = cameras[activeCam].GetViewMatrix();
    projection = glm::perspective(glm::radians(45.0f), float(Window::SRC_WIDTH) / float(Window::SRC_HEIGHT), 0.1f, 100.0f);

    /*
    sp1->Use();
    sp1->AddUniformVariable("uTransform");
    glUniformMatrix4fv(sp1->GetUniformId("uTransform"), 1, GL_FALSE, glm::value_ptr(transform));
    
    sp1->AddUniformVariable("uView");
    glUniformMatrix4fv(sp1->GetUniformId("uView"), 1, GL_FALSE, glm::mat4(1));
    sp1->AddUniformVariable("uProjection");
    glUniformMatrix4fv(sp1->GetUniformId("uProjection"), 1, GL_FALSE, glm::value_ptr(transform));
    */

    sp2->Use();

    sp2->SetMat4("uView", view);
    sp2->SetMat4("uProjection", projection);

    shaderLamp->SetMat4("uView", view);
    shaderLamp->SetMat4("uProjection", projection);
    shaderLamp->SetMat4("uTransform", glm::mat4(1));




    // DirectionLight dirLight{glm::vec3(-10.0f, -10.0f, -10.3f),1.0f, 0.07f, 0.032f, glm::vec3(1.0f), glm::vec3(0.9f), glm::vec3(0.75f)};

    SpotLight spotLight{
        cameras[activeCam]._camPosition, cameras[activeCam]._camFront,
        glm::cos(glm::radians(12.0f)), glm::cos(glm::radians(15.0f)),
        1.0f, 0.07f, 0.032f,
        glm::vec3(0.2f), glm::vec3(10.0f), glm::vec3(10.0f)
    };

    mainJ.Update();
    if (mainJ.IsPresent())
    {
        std::cout << mainJ.GetName() <<  " is present" << std::endl;
    }
    else
    {
        std::cout << "No joystick present"<<std::endl;
    }


    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    Cube cubes[10];
    for (unsigned int i = 0; i < 10; i++) {
        cubes[i] = Cube(Material::green_rubber, cubePositions[i], glm::vec3(1.0f));
        cubes[i].Init();
    }

    glm::vec3 pointLightPositions[] = {
            glm::vec3(0.7f,  0.2f,  2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3(0.0f,  0.0f, -3.0f)
    };
    Lamp lamps[4] = { Lamp(glm::vec3(1.0f), glm::vec3(0.5f), glm::vec3(0.8f), glm::vec3(1.0f), 1.0f, 0.07f, 0.032f, pointLightPositions[0], glm::vec3(0.25f)),
        Lamp(glm::vec3(1.0f), glm::vec3(0.5f), glm::vec3(0.8f), glm::vec3(1.0f), 1.0f, 0.07f, 0.032f, pointLightPositions[1], glm::vec3(0.25f)),
        Lamp(glm::vec3(1.0f), glm::vec3(0.5f), glm::vec3(0.8f), glm::vec3(1.0f), 1.0f, 0.07f, 0.032f, pointLightPositions[2], glm::vec3(0.25f)),
        Lamp(glm::vec3(1.0f), glm::vec3(0.5f), glm::vec3(0.8f), glm::vec3(1.0f), 1.0f, 0.07f, 0.032f, pointLightPositions[3], glm::vec3(0.25f))
    };
    for (unsigned int i = 0; i < 4; i++) {
        //lamps[i] = Lamp(glm::vec3(1.0f), glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f), 1.0f, 0.07f, 0.032f, pointLightPositions[i], glm::vec3(0.25f));
        lamps[i].Init();
    }


    lastframe = glfwGetTime();
    
    while (!window.ShouldClose())
    {
        double currentTime = glfwGetTime();
        deltatime = currentTime - lastframe;
        lastframe = currentTime;
        processInput(deltatime);

        window.Update();
        


        /* 
        sp1->Use();

        transform = glm::rotate(glm::mat4(1), glm::radians((float)glfwGetTime() / 5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(sp1->GetUniformId("uTransform"), 1, GL_FALSE, glm::value_ptr(transform));
        
        *No ebo
        glDrawArrays(GL_TRIANGLES, 0, 3);
        */



        // set default values for camera on 3d 
        sp2->Use();
        sp2->SetVec3("viewPos", cameras[activeCam]._camPosition);

        view = cameras[activeCam].GetViewMatrix();
        projection = glm::perspective(glm::radians(cameras[activeCam]._zoom), float(Window::SRC_WIDTH) / float(Window::SRC_HEIGHT), 0.1f, 100.0f);

        sp2->SetMat4("uView", view);
        sp2->SetMat4("uProjection", projection);

        // render lights
        // dirLight._direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(15.0f * deltatime), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(dirLight._direction, 1.0f));
        // dirLight.Render(sp2);

        // point lights
        for (int i = 0; i < 4; i++)
        {
            lamps[i]._pointlight.Render(sp2, i);
        }
        sp2->SetInt("countPointLights", 4);

        // spot lights
        if (flashLightOn)
        {
            spotLight._position = cameras[activeCam]._camPosition - (cameras[activeCam]._camFront * 0.0f);
            spotLight._direction = cameras[activeCam]._camFront;
            spotLight.Render(sp2, 0);
            sp2->SetInt("countSpotLights", 1);
        }
        else
        {
            sp2->SetInt("countSpotLights", 0);
        }




        // render meshes

        for (int i = 0; i < 10; i++)
        {
            cubes[i].Render(sp2);
        }

        shaderLamp->Use();
        shaderLamp->SetMat4("uView", view);
        shaderLamp->SetMat4("uProjection", projection);
        for (int i = 0; i < 4; i++)
        {
            lamps[i].Render(shaderLamp);
        }


        window.NewFrame();
    }

    for (int i = 0; i < 10; i++)
    {
        cubes[i].CleanUp();
    }

    for (int i = 0; i < 4; i++)
    {
        lamps[i].CleanUp();
    }

    delete sp1;
    delete sp2;
    delete shaderLamp;
    glfwTerminate();
    return 0;
}
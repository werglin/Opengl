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
#include"Model.hpp"
#include"Time.hpp"

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
bool flashLightOn = false;

Model m(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.02f));

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


    if (Mouse::s_ButtonDown(GLFW_MOUSE_BUTTON_1))
    {
        m.IncreaseBoneId();
    }

    if (Mouse::s_ButtonDown(GLFW_MOUSE_BUTTON_2))
    {
        window.SetCursor(false); // disable cursor
    }
    if (Mouse::s_ButtonUp(GLFW_MOUSE_BUTTON_2))
    {
        window.SetCursor(true); // enable cursor
    }
    

    if (!window._cursorEnabled)
    {
        double dx = Mouse::s_GetMouseDX(), dy = Mouse::s_GetMouseDY();
        if (dx != 0 || dy != 0)
        {
            cameras[activeCam].UpdateCameraDirection(dx * dt * 150.0f, dy * dt * 150.0f);
        }

        double scroolDy = Mouse::s_GetScrollDY();
        if (scroolDy != 0)
        {
            cameras[activeCam].UpdateCameraZoom(scroolDy);
        }
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


    ShaderProgram* fbxshader, * sp2, * shaderLamp;
    fbxshader = new ShaderProgram();
    sp2 = new ShaderProgram();
    shaderLamp = new ShaderProgram();

    fbxshader->AttachShader("assets/shaders/fbxshader_vs.glsl", GL_VERTEX_SHADER);
    fbxshader->AttachShader("assets/shaders/fbxshader_fs.glsl", GL_FRAGMENT_SHADER);
    fbxshader->Link();

    sp2->AttachShader("assets/shaders/simplevertex.glsl", GL_VERTEX_SHADER);
    sp2->AttachShader("assets/shaders/simplefragmentv2.glsl", GL_FRAGMENT_SHADER);
    sp2->Link();

    shaderLamp->AttachShader("assets/shaders/simplevertex.glsl", GL_VERTEX_SHADER);
    shaderLamp->AttachShader("assets/shaders/lamp_fs.glsl", GL_FRAGMENT_SHADER);
    shaderLamp->Link();


    glm::mat4 view = glm::mat4(1);
    glm::mat4 projection = glm::mat4(1);

    view = cameras[activeCam].GetViewMatrix();
    projection = glm::perspective(glm::radians(45.0f), float(Window::SRC_WIDTH) / float(Window::SRC_HEIGHT), 0.1f, 100.0f);


    sp2->Use();

    sp2->SetMat4("uView", view);
    sp2->SetMat4("uProjection", projection);

    shaderLamp->SetMat4("uView", view);
    shaderLamp->SetMat4("uProjection", projection);
    shaderLamp->SetMat4("uTransform", glm::mat4(1));


    DirectionLight dirLight;
    dirLight._direction = glm::vec3(-0.2f, -0.9f, -0.2f);
    dirLight._ambient = glm::vec4(1.0f);
    dirLight._diffuse = glm::vec4(1.0f);
    dirLight._specular = glm::vec4(0.7f);

    SpotLight spotLight{
        cameras[activeCam]._camPosition, cameras[activeCam]._camFront,
        glm::cos(glm::radians(6.0f)), glm::cos(glm::radians(8.0f)),
        1.0f, 0.07f, 0.032f,
        glm::vec4(0.2f,0.2f,0.2f,1.0f), glm::vec4(10.0f), glm::vec4(10.0f)
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
    Lamp lamps[4] = { 
        Lamp(glm::vec3(1.0f), glm::vec4(0.5f), glm::vec4(0.8f), glm::vec4(1.0f), 1.0f, 0.07f, 0.032f, pointLightPositions[0], glm::vec3(0.25f)),
        Lamp(glm::vec3(1.0f), glm::vec4(0.5f), glm::vec4(0.8f), glm::vec4(1.0f), 1.0f, 0.07f, 0.032f, pointLightPositions[1], glm::vec3(0.25f)),
        Lamp(glm::vec3(1.0f), glm::vec4(0.5f), glm::vec4(0.8f), glm::vec4(1.0f), 1.0f, 0.07f, 0.032f, pointLightPositions[2], glm::vec3(0.25f)),
        Lamp(glm::vec3(1.0f), glm::vec4(0.5f), glm::vec4(0.8f), glm::vec4(1.0f), 1.0f, 0.07f, 0.032f, pointLightPositions[3], glm::vec3(0.25f)),
    };
    for (unsigned int i = 0; i < 4; i++) {
        //lamps[i] = Lamp(glm::vec3(1.0f), glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f), 1.0f, 0.07f, 0.032f, pointLightPositions[i], glm::vec3(0.25f));
        lamps[i].Init();
    }



    // fbx model
    //Model m(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.2f), false);
    // Model m(glm::vec3(0.0f,0.0f,-5.0f), glm::vec3(0.2f), false);
    m.LoadModel("Punching.fbx");
    // m.LoadModel("boneehehe.fbx");
    // m.LoadModel("ggahahaxdlol.fbx");
    // Model m(glm::vec3(0.0f,0.0f,-5.0f), glm::vec3(0.2f), true);
    // m.LoadModel("Hip Hop Dancing.fbx");



    

    
    float frametime = 1.0f / 144.0f; // fixed 144 fps
    float passedtime = 0.0f;
    Time time;
    while (!window.ShouldClose())
    {
        // fps limiter
        if (passedtime < frametime)
        {
            passedtime += time.ElapsedTimeInSeconds();
            continue;
        }
        Time::s_deltaTime = passedtime;
        passedtime = 0.0f;
        
        processInput(Time::s_deltaTime);

        window.Update();
        


        /* 

        transform = glm::rotate(glm::mat4(1), glm::radians((float)glfwGetTime() / 5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(sp1->GetUniformId("uTransform"), 1, GL_FALSE, glm::value_ptr(transform));
        
        *No ebo
        glDrawArrays(GL_TRIANGLES, 0, 3);
        */


        // --- sp2 shader render
        // set default values for camera on 3d 
        sp2->Use();
        sp2->SetVec3("viewPos", cameras[activeCam]._camPosition);

        view = cameras[activeCam].GetViewMatrix();
        projection = glm::perspective(glm::radians(cameras[activeCam]._zoom), float(Window::SRC_WIDTH) / float(Window::SRC_HEIGHT), 0.1f, 100.0f);

        sp2->SetMat4("uView", view);
        sp2->SetMat4("uProjection", projection);

        // render lights
        dirLight._direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(15.0f * Time::s_deltaTime), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(dirLight._direction, 1.0f));
        dirLight.Render(sp2);

        // point lights
        for (int i = 0; i < 4; i++)
        {
            lamps[i]._pointlight.Render(sp2, i);
        }
        sp2->SetInt("countPointLights", 4);

        // spot lights
        if (flashLightOn)
        {
            // dunno why but light moves more than cam like if we move 1,1 with cam - light moves 1.5,1.7 
            spotLight._position = cameras[activeCam]._camPosition;
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

        /* ------------------------------------- */

        // --- shaderLamp shader render
        shaderLamp->Use();
        shaderLamp->SetMat4("uView", view);
        shaderLamp->SetMat4("uProjection", projection);
        for (int i = 0; i < 4; i++)
        {
            lamps[i].Render(shaderLamp);
        }
        /*---------------------------------------------------*/

        // --- fbxshader shader render

        fbxshader->Use();

        fbxshader->SetVec3("viewPos", cameras[activeCam]._camPosition);

        view = cameras[activeCam].GetViewMatrix();
        projection = glm::perspective(glm::radians(cameras[activeCam]._zoom), float(Window::SRC_WIDTH) / float(Window::SRC_HEIGHT), 0.1f, 100.0f);

        fbxshader->SetMat4("uView", view);
        fbxshader->SetMat4("uProjection", projection);

        // render lights
        dirLight._direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(15.0f * Time::s_deltaTime), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(dirLight._direction, 1.0f));
        dirLight.Render(fbxshader);

        // point lights
        for (int i = 0; i < 4; i++)
        {
            lamps[i]._pointlight.Render(fbxshader, i);
        }
        fbxshader->SetInt("countPointLights", 4);

        // spot lights
        if (flashLightOn)
        {
            // dunno why but light moves more than cam like if we move 1,1 with cam - light moves 1.5,1.7 
            spotLight._position = cameras[activeCam]._camPosition;
            spotLight._direction = cameras[activeCam]._camFront;
            spotLight.Render(fbxshader, 0);
            fbxshader->SetInt("countSpotLights", 1);
        }
        else
        {
            fbxshader->SetInt("countSpotLights", 0);
        }



        fbxshader->SetInt("hasBones", m._hasAnims);
        if (m._hasAnims)
        {
            std::vector<glm::mat4> Transforms;
            m.GetBoneTransforms(time.ApplicationElapsedInSeconds(), Transforms);


            for (unsigned int i = 0; i < Transforms.size(); i++) {
                fbxshader->SetMat4("gBones[" + std::to_string(i) + "]", Transforms[i]);
            }
        }
        
        // render meshes
        m.Render(fbxshader);

        window.NewFrame();
    }

    m.CleanUp();

    for (int i = 0; i < 10; i++)
    {
        cubes[i].CleanUp();
    }

    for (int i = 0; i < 4; i++)
    {
        lamps[i].CleanUp();
    }
    fbxshader->CleanUp();
    sp2->CleanUp();
    shaderLamp->CleanUp();
    delete fbxshader;
    delete sp2;
    delete shaderLamp;
    glfwTerminate();
    return 0;
}
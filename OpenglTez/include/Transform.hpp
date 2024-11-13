#ifndef TRANSFORM_hpp
#define TRANSFORM_hpp

#include<glm/mat4x4.hpp>
#include<glm/vec3.hpp>

class Transform
{
public:

    Transform();

    glm::mat4& getMatrix();
    glm::vec3& getPosition();
    glm::vec3& getScale();
    glm::vec3& getEulerAngles();

    void setPosition(const glm::vec3& position);
    void setScale(const glm::vec3& scale);
    void setEulerAngles(const glm::vec3& angles);
    void update();

protected:
    glm::vec3 _position;
    glm::vec3 _scale;
    glm::vec3 _eulerAngles;
    glm::mat4 _mtxTransform;
private:

};

#endif
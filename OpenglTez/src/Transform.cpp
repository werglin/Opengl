#include"transform.hpp"

#include<glm/gtc/matrix_transform.hpp>
Transform::Transform()
{
    _position = glm::vec3(0.0f, 0.0f, 0.0f);
    _scale = glm::vec3(1.0f, 1.0f, 1.0f);
    _eulerAngles = glm::vec3(0.0f, 0.0f, 0.0f);
    _mtxTransform = glm::mat4(1);
}

glm::vec3& Transform::getEulerAngles()
{
    return _eulerAngles;
}

glm::vec3& Transform::getPosition()
{
    return _position;
}

glm::vec3& Transform::getScale()
{
    return _scale;
}

glm::mat4& Transform::getMatrix()
{
    return _mtxTransform;
}

void Transform::setEulerAngles(const glm::vec3& angles)
{
    _eulerAngles = angles;
}

void Transform::setPosition(const glm::vec3& position)
{
    _position = position;
}

void Transform::setScale(const glm::vec3& scale)
{
    _scale = scale;
}

void Transform::update()
{
    glm::mat4 mtxTranslate = glm::translate(glm::mat4(1), _position);
    glm::mat4 mtxScale = glm::scale(glm::mat4(1), _scale);

    // zyx

    glm::mat4 mtxRotX = glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 mtxRotY = glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 mtxRotZ = glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 mtxRot = mtxRotZ * mtxRotY * mtxRotX;
    // Translate Rotate Scale - TRS
    _mtxTransform = mtxTranslate * mtxRot * mtxScale;

}

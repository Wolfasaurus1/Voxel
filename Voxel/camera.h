#ifndef CAMERA_H
#define CAMERA_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>

#include <vector>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT, 
    UP, 
    DOWN
};


class Camera
{
public:

    glm::quat orientation, targetOrientation;
    glm::vec3 position, targetPosition;

    Camera()
    {
        position = glm::vec3(-20.0f, -40.0f, -20.0f);
        targetPosition = position;
        orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        targetOrientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::mat4_cast(orientation) * glm::translate(glm::mat4(1.0f), position);
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f) * orientation;
        float speed = 50.0f;

        if (direction == FORWARD) {
            targetPosition -= glm::normalize(front * glm::vec3(1.0f, 0.0f, 1.0f)) * deltaTime * speed;
        }
        else if (direction == BACKWARD) {
            targetPosition += glm::normalize(front * glm::vec3(1.0f, 0.0f, 1.0f)) * deltaTime * speed;
        } 
        else if (direction == LEFT) {
            targetPosition -= glm::normalize(glm::vec3(front.z, 0, -front.x)) * deltaTime * speed;
        }
        else if (direction == RIGHT) {
            targetPosition += glm::normalize(glm::vec3(front.z, 0, -front.x)) * deltaTime * speed;
        }
        else if (direction == UP) {
            targetPosition.y -= deltaTime * speed;
        }
        else if (direction == DOWN) {
            targetPosition.y += deltaTime * speed;
        }
    }

    void ProcessMouseMovement(float xoffset, float yoffset)
    {
        float pitch = -yoffset * 0.1f;
        float yaw = xoffset * 0.1f;

        glm::quat pitchQuat = glm::angleAxis(glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat yawQuat = glm::angleAxis(glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));

        targetOrientation = glm::normalize(pitchQuat) * targetOrientation * glm::normalize(yawQuat);
    }

    void roll() {
        //glm::quat roll = glm::angleAxis(glm::radians(0.8f), glm::vec3(0.0f, 0.0f, 1.0f));

        //targetOrientation = roll * targetOrientation;
    }

    void update()
    {
        orientation = targetOrientation;//glm::slerp(orientation, targetOrientation, 0.75f);
        position = targetPosition;//glm::lerp(position, targetPosition, 0.8f);
    }
};
#endif


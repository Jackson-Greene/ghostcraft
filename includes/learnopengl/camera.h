//sourced from https://learnopengl.com/
//more reference details provided in report

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement 
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  10.0f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;
    // Euler Angles
    float yaw;
    float pitch;
    // Camera options
    float movement_speed;
    float mouse_sensitivity;

    // Constructor with vectors
    Camera(glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up_ = glm::vec3(0.0f, 1.0f, 0.0f), float yaw_ = YAW, float pitch_ = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY)
    {
        position = position_;
        world_up = up_;
        yaw = yaw_;
        pitch = pitch_;
        updateCameraVectors();
    }

    //Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(position, position + front, up);
    }

    glm::vec3 getPosition()
    {
        return(position);
    }

    void setPosition(glm::vec3 position_)
    {
        position = position_;
    }

    void setXPosition(float x)
    {
        position.x = x;
    }

    void setYPosition(float y)
    {
        position.y = y;
    }

    void setZPosition(float z)
    {
        position.z = z;
    }

    void processKeyboard(Camera_Movement direction, float delta_time)
    {
        float velocity = movement_speed * delta_time;

        if(direction == FORWARD)
        {
            position += front * velocity;
        }
        if(direction == BACKWARD)
        {
            position -= front * velocity;
        }
        if(direction == LEFT)
        {
            position -= right * velocity;
        }
        if(direction == RIGHT)
        {
            position += right * velocity;
        }
    }


    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement(float x_offset, float y_offset, GLboolean constrain_pitch = true)
    {
        x_offset *= mouse_sensitivity;
        y_offset *= mouse_sensitivity;

        //full rotation can be desribed by 0 - 360 degrees
        yaw = glm::mod((yaw + x_offset), 360.0f);
        pitch += y_offset;

        //can't look backwards by looking too far up or down
        if (constrain_pitch)
        {
            if(pitch > 89.0f)
            {
                pitch = 89.0f;
            }
                
            if(pitch < -89.0f)
            {
                pitch = -89.0f;
            }
        }

        // update front, right and up Vectors using the updated Euler angles
        updateCameraVectors();
    }

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        //calculate new vectors
        glm::vec3 front_;
        front_.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front_.y = sin(glm::radians(pitch));
        front_.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        //set to front
        front = glm::normalize(front_);
        
        //update right and up also
        right = glm::normalize(glm::cross(front, world_up));  //normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up = glm::normalize(glm::cross(right, front));
    }
};
#endif
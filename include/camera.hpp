#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{
private:
    void cal_front();
public:
    glm::vec3 position;
    glm::vec3 front;//注意这是指向摄像机的方向，不是摄像机看向的方向
    glm::vec3 up;
    float pitch,yaw;
    float fov;

    Camera();
    Camera(glm::vec3 pos,float _pitch,float _yaw,float _fov);

    glm::mat4 lookAt();
    glm::mat4 lookAt(glm::vec3 target);

    void mouse_control(float xoffset,float yoffset,float sensitivity);
};

#endif
#include<camera.hpp>

Camera::Camera(){
    position = glm::vec3(0.0f, 0.0f,  3.0f);
    front = glm::vec3(1.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f,  0.0f);
    pitch=0,yaw=0;//俯仰角,
    fov=45;
}

Camera::Camera(glm::vec3 pos,float _pitch,float _yaw,float _fov):position(pos),pitch(_pitch),yaw(_yaw),fov(_fov){
    cal_front();
    glm::vec3 right = glm::normalize(glm::cross(
        glm::vec3(0.0f,1.0f,0.0f), front));
    up = glm::cross(front, right);
}

void Camera::cal_front(){
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front=glm::normalize(front);
}

glm::mat4 Camera::lookAt(){
    return glm::lookAt(position,position+front, up); 
}

glm::mat4 Camera::lookAt(glm::vec3 target){
    return glm::lookAt(position,target, up); 
}

void Camera::mouse_control(float xoffset,float yoffset,float sensitivity = 0.05){
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    cal_front();
}
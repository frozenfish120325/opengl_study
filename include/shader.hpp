#ifndef SHADER_H
#define SHADER_H

#define SHADER_PATH "./src/shader/"
#define V_SHADER_EXTENSION ".v_shader"
#define F_SHADER_EXTENSION ".f_shader"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
private:
    // shader路径
    static const char*path;
    // 程序ID
    unsigned int ID;

    void checkCompileErrors(unsigned int shader, std::string type);
public:
    unsigned int get_id(){return ID;}
    // 构造器读取并构建着色器
    Shader(std::string name);
    // 使用/激活程序
    void use();
    // uniform工具函数
    void set(const char*name, bool) const;  
    void set(const char*name, int) const;   
    void set(const char*name, float) const;
    void set(const char*name, float,float,float) const;
    void set(const char*name, float,float,float,float) const;
    void set(const char*name, glm::mat4) const;
};

#endif
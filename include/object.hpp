#ifndef OBJECT_H
#define OBJECT_H

#include<functional>
#include<shader.hpp>

class Objetct{
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int vertices_num;
public:
    Shader*shader;
    Objetct(float*vertices,unsigned int*indices,unsigned int Attrib[],int attrib_nums,Shader*);
    Objetct(float*vertices,unsigned int Attrib[],int attrib_nums,Shader*);
    void render(std::function<void(Shader*)>);
    unsigned int get_VAO(){return VAO;}
};

#endif
#include<object.hpp>
#include<glad/glad.h>

Objetct::Objetct(float*vertices,unsigned int*indices,unsigned int attrib[],int attrib_nums,Shader*_shader):shader(_shader){
    int vertices_size=attrib[attrib_nums-2];
    int indices_size=attrib[attrib_nums-1];

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);//封装接下来VBO和EBO的配置，记录顶点属性指针
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  //绑定VBO
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

    //设置顶点属性指针
        //顶点属性layout(location)位置值
        //这个顶点属性组有几个数据值
        //数据值的类型
        //是否希望数据被标准化
        //一个顶点所有属性组的字节数
        //数据在缓冲中起始位置的偏移量
    //将顶点属性location(0)应用到当前绑定的VBO

    unsigned long val_num=0;
    for(unsigned int i=0;i<attrib_nums-2;i++)val_num+=attrib[i];
    size_t offset=0;
    for(unsigned int i=0;i<attrib_nums-2;i++){
        glVertexAttribPointer(i, attrib[i], GL_FLOAT, GL_FALSE, val_num * sizeof(float), (void*)offset);
        glEnableVertexAttribArray(i);
        offset+=attrib[i] * sizeof(float);
    }
    
    vertices_num=indices_size/sizeof(indices[0]);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
}

Objetct::Objetct(float *vertices, unsigned int attrib[], int attrib_nums, Shader *_shader):shader(_shader)
{
    int vertices_size=attrib[attrib_nums-1];
    EBO=0;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  //绑定VBO
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

    unsigned long val_num=0;
    for(unsigned int i=0;i<attrib_nums-1;i++)val_num+=attrib[i];
    size_t offset=0;
    for(unsigned int i=0;i<attrib_nums-1;i++){
        glVertexAttribPointer(i, attrib[i], GL_FLOAT, GL_FALSE, val_num * sizeof(float), (void*)offset);
        glEnableVertexAttribArray(i);
        offset+=attrib[i] * sizeof(float);
    }
    
    vertices_num=vertices_size/sizeof(vertices[0])/val_num;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
}

void Objetct::render(std::function<void(Shader*)>func)
{
    shader->use();
    
    func(shader);
    
    glBindVertexArray(VAO);
    if(EBO!=0){
        glDrawElements(GL_TRIANGLES, vertices_num, GL_UNSIGNED_INT, 0);
    }
    else{
        glDrawArrays(GL_TRIANGLES, 0, vertices_num);
    }
}

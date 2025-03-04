//stdlib
#include<iostream>
#include<memory>
#include<functional>
#include<vector>
//opengl_lib
#include <glad/glad.h>
#include <glfw/glfw3.h>
//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//other_lib
#include<stb_image.h>
//my_lib
#include<shader.hpp>
#include<object.hpp>
#include<camera.hpp>
//resource
#include"vertices.cpp"

#define DEF_WINDOW_WIDTH 1000
#define DEF_WINDOW_HEIGHT 1000

#define TEXTURE_PATH "./src/texture/"

//回调：窗口大小变化
static int window_width=DEF_WINDOW_WIDTH;
static int window_height=DEF_WINDOW_WIDTH;
void framebuffer_size_callback(GLFWwindow*,int width, int height);
//初始化GLFW和FLAD
GLFWwindow*init_window(int width,int height,const char*name);
//获取deltatime
float deltatime(){
    static float deltaTime = 0.0f;
    static float lastFrame = 0.0f;
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    return deltaTime;
}
//处理键盘输入
void processInput(GLFWwindow *window,std::vector<std::function<void(GLFWwindow*)>>&);
//鼠标移动回调
static bool firstMouse=true;
static float xoffset=0,yoffset=0;
static bool mouse_move=false;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//鼠标滚动回调
static float s_xoffset=0,s_yoffset=0;
static bool mouse_scroll=false;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    mouse_scroll=true;
    s_yoffset=yoffset;
}

//生成贴图
unsigned int get_texture(std::string);


extern float vertices[];
// extern unsigned int indices[];
// unsigned int attr[]={3,3,2,sizeof(vertices),sizeof(indices)};
unsigned int attr[]={3,2,sizeof(vertices)};
extern glm::vec3 cubePositions[];


int main(){
    //初始化
    GLFWwindow* window=init_window(DEF_WINDOW_WIDTH, DEF_WINDOW_HEIGHT,"LearnOpenGL");
    if (window == NULL)return 1;

    //设置
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//绘制模式
    stbi_set_flip_vertically_on_load(true);//贴图坐标反转
    glEnable(GL_DEPTH_TEST);//深度测试
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//隐藏鼠标

    //shader和obj
    Shader myShader{"basic_2"};
    Objetct myObj{vertices,attr,sizeof(attr)/sizeof(unsigned int),&myShader};

    //贴图
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,get_texture("test.jpg"));
    myShader.set("ourTexture",0);
    
    //相机
    Camera camera{glm::vec3(0.0,0.0,-5.0),0,90,45};

    //键盘输入事件
    std::vector<std::function<void(GLFWwindow*)>>keyboard_actions;
        //相机位置
    keyboard_actions.emplace_back([&camera](GLFWwindow*window){
        float cameraSpeed = 5.0f;
        cameraSpeed*=deltatime();
        //FB
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.position += cameraSpeed * camera.front;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.position -= cameraSpeed * camera.front;
        //LR
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
        //UD
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camera.position += camera.up * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camera.position -= camera.up * cameraSpeed;
    });

    //鼠标输入事件
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //渲染循环
    while(!glfwWindowShouldClose(window))
    {
        //清屏
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //处理键盘输入
        processInput(window,keyboard_actions);

        //处理相机
        if(mouse_move){
            camera.mouse_control(xoffset,yoffset,0.1f);
            // std::cout<<camera.yaw<<std::endl;
            //重置鼠标偏移量
            xoffset=0,yoffset=0;
            mouse_move=false;
        }
        if(mouse_scroll){
            if(camera.fov >= 10.0f && camera.fov <= 75.0f)
                camera.fov -= s_yoffset;
            if(camera.fov <= 10.0f)
                camera.fov = 10.0f;
            if(camera.fov >= 75.0f)
                camera.fov = 75.0f;
            mouse_scroll=false;
        }
        glm::mat4 view=camera.lookAt();
        glm::mat4 projection = glm::perspective(glm::radians(camera.fov),
         (float)window_width / window_height, 0.1f, 100.0f);

        //渲染队列
        for(unsigned int i = 0; i < 10; i++)
        {
            //变换矩阵
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i; 
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            //传入变换矩阵，渲染
            glm::mat4 trans = projection*view*model*glm::mat4(1.0f);
            myObj.render([trans](Shader*shader)->void{
                shader->set("transform",trans);
            });
        }
        

        glfwSwapBuffers(window);//刷新
        glfwPollEvents();//检查触发事件
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    window_width=width;
    window_height=height;
    glViewport(0, 0, width, height);
}
GLFWwindow*init_window(int width,int height,const char*name){
    //GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //window
    GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();//关闭glfw
        return NULL;
    }
    glfwMakeContextCurrent(window);

    //GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }
    
    //设置视口和视口回调
    glViewport(0, 0, DEF_WINDOW_WIDTH, DEF_WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    return window;
} 

void processInput(GLFWwindow *window,std::vector<std::function<void(GLFWwindow*)>>&action_lists)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    for(auto action:action_lists){
        action(window);
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    static float lastX = DEF_WINDOW_WIDTH/2, lastY = DEF_WINDOW_HEIGHT/2;
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    xoffset = xpos - lastX;
    yoffset = lastY - ypos;
    lastX=xpos;
    lastY=ypos;
    mouse_move=true;
}

unsigned int get_texture(std::string file){
    std::string file_path=std::string{TEXTURE_PATH}+file;
    
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    int width, height, nrChannels;
    unsigned char *data = stbi_load(file_path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return 0;
    }
    stbi_image_free(data);
    return texture;
}
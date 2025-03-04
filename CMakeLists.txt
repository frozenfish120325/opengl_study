cmake_minimum_required(VERSION 3.10)
project(OpenGLExample)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

# 添加可执行文件
add_executable(${PROJECT_NAME}
    ./src/glad.c
    ./src/shader.cpp
    ./src/camera.cpp
    ./src/object.cpp
    ./src/stb_image.cpp
    ./src/main.cpp
)

# 添加自定义命令到MyApp的构建前步骤
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
# COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/bin/
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/shader/ ${CMAKE_BINARY_DIR}/bin/src/shader/
COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/texture/ ${CMAKE_BINARY_DIR}/bin/src/texture/
    COMMENT "undating src..."
)

# 链接库
target_link_libraries(${PROJECT_NAME}
    PRIVATE glfw3
)

target_compile_options(${PROJECT_NAME} PRIVATE -finput-charset=UTF-8 -fexec-charset=GBK)
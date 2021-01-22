#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "controls.hpp"

using namespace glm;
    
void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

class RenderEngine{
    private:
        GLFWwindow *window {nullptr};
        GLuint vertexBuffer {};
        GLuint colorBuffer {};
        GLuint programId {};
        GLuint matrixId {};

        std::vector<glm::vec3> vertex {};
        std::vector<glm::vec3> color {};

        void setupGlfw();
        void setupGlew();
        void initBuffers();
        void loadShaders();
        void readShaders(const std::string& path, const GLuint& shader_type, GLuint& id);

    public:
        RenderEngine(){
            setupGlfw();
            setupGlew();
            initBuffers();
            loadShaders();

            matrixId = glGetUniformLocation(programId, "MVP");

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glDepthFunc(GL_LESS);
        };
        RenderEngine(GLFWwindow *_window){
            window = _window;
            setupGlew();
            initBuffers();
            loadShaders();

            matrixId = glGetUniformLocation(programId, "MVP");

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            glDepthFunc(GL_LESS);
        }

        void render();
        void update_buffer(cv::Mat &data);
};

void RenderEngine::setupGlfw(){
    if(!glfwInit()){
        std::runtime_error("Could not initialize GLFW!");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(800, 600, "MRI reconstruction", nullptr, nullptr);

    if(window == nullptr){
        glfwTerminate();
        std::runtime_error("Could not create GLFW Window");
    }

    glfwMakeContextCurrent(window);
}

void RenderEngine::setupGlew(){
    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK){
        std::runtime_error("Could not initialize GLEW");
    }
}

void RenderEngine::initBuffers(){
    GLuint VertexArrayID {0};
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &colorBuffer);
}

void RenderEngine::loadShaders(){
    GLuint shaderId {};
    GLuint fragmentId {};
    readShaders("/home/meschuge/workspace/mri_visualization/MRI_reconstruction/src/ContextWindow/shaders/vert.shader", GL_VERTEX_SHADER, shaderId);
    readShaders("/home/meschuge/workspace/mri_visualization/MRI_reconstruction/src/ContextWindow/shaders/frag.shader", GL_FRAGMENT_SHADER, fragmentId);

    programId = glCreateProgram();
    glAttachShader(programId, shaderId);
    glAttachShader(programId, fragmentId);
    glLinkProgram(programId);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    glGetShaderiv(programId, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(programId, GL_INFO_LOG_LENGTH, &InfoLogLength);

    glDetachShader(programId, shaderId);
    glDetachShader(programId, fragmentId);

    glDeleteShader(shaderId);
    glDeleteShader(fragmentId);    
}

void RenderEngine::readShaders(const std::string& path, const GLuint& shader_type, GLuint& id){
    id = glCreateShader(shader_type);
    std::string shaderCode {};
    std::ifstream shaderStream(path, std::ios::in);
    if(shaderStream.is_open()){
        std::stringstream ss;
        ss << shaderStream.rdbuf();
        shaderCode = ss.str();
        shaderStream.close();
    } else{
        std::runtime_error("Could not read shader");
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    char const * sourcePointer {shaderCode.c_str()};
    glShaderSource(id, 1, &sourcePointer, nullptr);
    glCompileShader(id);

    glGetShaderiv(id, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
}   


void RenderEngine::render(){
    computeMatricesFromInput(window);

    mat4 projection {getProjectionMatrix()};
    mat4 view {getViewMatrix()};
    mat4 model {mat4(1.0)};
    mat4 mvp {projection * view * model};
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programId);
    glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glPointSize(3.0f);
    glDrawArrays(GL_POINTS, 0, vertex.size());
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void RenderEngine::update_buffer(cv::Mat &data){
    for(int i {0}; i < data.rows; ++i){
        for(int j {0}; j < data.cols; ++j){
            if(*data.ptr(j,i) > 5){
                vertex.push_back(glm::vec3(i, j, 1));
                color.push_back(glm::vec3(i,j,0));
            }
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(glm::vec3), &vertex[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(glm::vec3), &color[0], GL_DYNAMIC_DRAW);
};
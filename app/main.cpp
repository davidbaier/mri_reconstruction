#include <iostream>
#include <sstream>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <opencv4/opencv2/highgui.hpp>

#include "../src/MRIFile/Storage.hpp"
#include "../src/ContextWindow/render.hpp"

using namespace std;
using namespace cv;

GLFWwindow* createGLFWWindow(){
    GLFWwindow *window;
    if(!glfwInit()){
        std::runtime_error("Could not initialize GLFW!");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    window = glfwCreateWindow(800, 600, "MRI reconstruction", nullptr, nullptr);

    if(window == nullptr){
        glfwTerminate();
        std::runtime_error("Could not create GLFW Window");
    }

    glfwMakeContextCurrent(window);
    return window;
}

int main(int argc, char** argv) {
    if(argc < 2){
        cout << "Usage: ./MRI_reconstruction PATH" << endl;
        return -1;
    }

    string path = argv[1];

    MRIStorage<MRIFile> storage;
    storage.load_data(path);

    GLFWwindow *window {createGLFWWindow()};
    RenderEngine render = RenderEngine(window);

    MRIStorage<MRIFile>::Iterator itr {storage.begin()};

    render.update_buffer(itr->data);
    do{
        render.render();
    }while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) ==  0);

    return 0;
} 

#ifndef COMMON_H_
#define COMMON_H_

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 position {150, 120, 600};
float horizontalAngle {3.15};
float verticalAngle {0.0};
float initial_fov {700.0};
float speed {10.0};
float mouseSpeed {0.01};

glm::mat4 view;
glm::mat4 projection;

void computeMatricesFromInput(GLFWwindow *window) {
    static double lastTime {glfwGetTime()};
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    glfwSetCursorPos(window, width/2, height/2);

    double currentTime {glfwGetTime()};
    float deltaTime = static_cast<float>(currentTime - lastTime);

    horizontalAngle += mouseSpeed * deltaTime *float(width/2  - xpos);
    verticalAngle += mouseSpeed * deltaTime * float(height/2 - ypos);

    glm::vec3 direction(cos(verticalAngle)*sin(horizontalAngle),
                        sin(verticalAngle),
                        cos(verticalAngle)*cos(horizontalAngle));

    glm::vec3 right(
        sin(horizontalAngle - 3.14f/2.0f),
        0,
        cos(horizontalAngle - 3.14f/2.0f));
    glm::vec3 up {glm::cross(right, direction)};

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS){
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS){
        position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS){
        position -= right * deltaTime * speed;
    }

    float FoV {initial_fov};

    projection = glm::perspective(glm::radians(FoV), (float)width/(float)height, 0.1f, 1000.0f);
    view = glm::lookAt(position, position+direction, up);

    lastTime = currentTime;
}

glm::mat4 getViewMatrix(){
    return view;
}
glm::mat4 getProjectionMatrix(){
    return projection;
}

#endif
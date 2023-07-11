#include "windowG7.h"
#include <iostream>

Window7::Window7(int w, int h, const std::string str, GLFWmonitor* monitor, GLFWwindow* sharedWindow) :
    width(w), height(h), firstMouse(1), camZoom(cam.zoom), deltaTime(0.0f), lastFrame(0.0f), fbMode(0), hold(0)
{

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    win = glfwCreateWindow(w, h, str.c_str(), monitor, sharedWindow);
    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }

    glfwSetWindowUserPointer(win, this); 
    glfwSetCursorPos(win, getWidth() / 2, getHeight() / 2);
    cursorX = (float)width / 2;
    cursorY = (float)height / 2;
}

Window7::~Window7()
{
    glfwDestroyWindow(win);
    glfwTerminate();
}

bool Window7::windowCreated()
{
    if (!win)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(win);
    return true;
}

bool Window7::keyPressedOnce(int key)
{
    bool result;
    
    return hold;
}

void Window7::swapBuffer()
{
    glfwSwapBuffers(win);
}

bool Window7::canClose() 
{
    return glfwWindowShouldClose(win);
}

glm::mat4 Window7::getViewMatrix()
{
    return cam.getViewMatrix();
}

glm::vec3 Window7::getViewPos()
{
    return cam.cameraPos;
}

glm::vec3 Window7::getViewFront()
{
    return cam.cameraFront;
}

void Window7::getDeltaTime()
{
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void Window7::keyHandle()
{
    getDeltaTime();
    glm::vec3 moveFront(cam.cameraFront.x, 0.0f, cam.cameraFront.z);
    float velocity = static_cast<float>(cam.cameraSpeed * deltaTime);
    if (keys[GLFW_KEY_UP])
        cam.cameraPos += velocity * moveFront;
    if (keys[GLFW_KEY_DOWN])
        cam.cameraPos -= velocity * moveFront;
    if (keys[GLFW_KEY_LEFT])
        cam.cameraPos -= glm::normalize(glm::cross(moveFront, cam.cameraUp)) * velocity;
    if (keys[GLFW_KEY_RIGHT])
        cam.cameraPos += glm::normalize(glm::cross(moveFront, cam.cameraUp)) * velocity;
    if (keys[GLFW_KEY_W])
        cam.cameraPos += cam.cameraUp * velocity;
    if (keys[GLFW_KEY_S]) {
        if (cam.cameraPos.y >= cam.charHeight)
            cam.cameraPos -= cam.cameraUp * velocity;
        else cam.cameraPos.y = cam.charHeight;
    }

}

void Window7::setCallback()
{
    glfwSetCursorPosCallback(win, &mousePosCallback);
    glfwSetMouseButtonCallback(win, &mouseButtonCallback);
    glfwSetScrollCallback(win, &scrollCallback);
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetKeyCallback(win, &keyCallback);
}

void Window7::mousePosCallback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    Window7* win_ptr = static_cast<Window7*>(glfwGetWindowUserPointer(window));
    float xoffset = xpos - win_ptr->cursorX;
    float yoffset = win_ptr->cursorY - ypos;

    xoffset *= win_ptr->cam.mouseSensitivity;
    yoffset *= win_ptr->cam.mouseSensitivity;

    win_ptr->cam.yaw += xoffset;
    win_ptr->cam.pitch += yoffset;

    if (win_ptr->cam.pitch > 89.0f)
        win_ptr->cam.pitch = 89.0f;
    if (win_ptr->cam.pitch < -89.0f)
        win_ptr->cam.pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(win_ptr->cam.yaw)) * cos(glm::radians(win_ptr->cam.pitch));
    front.y = sin(glm::radians(win_ptr->cam.pitch));
    front.z = sin(glm::radians(win_ptr->cam.yaw)) * cos(glm::radians(win_ptr->cam.pitch));
    win_ptr->cam.cameraFront = glm::normalize(front);

    glfwSetCursorPos(win_ptr->win, win_ptr->getWidth() / 2, win_ptr->getHeight() / 2);

}

void Window7::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Window7* win_ptr = static_cast<Window7*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            win_ptr->keys[GLFW_MOUSE_BUTTON_LEFT] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            win_ptr->keys[GLFW_MOUSE_BUTTON_LEFT] = false;
        }
    }
}

void Window7::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    float xOffset = static_cast<float>(xoffset);
    float yOffset = static_cast<float>(yoffset);
    Window7* win_ptr = static_cast<Window7*>(glfwGetWindowUserPointer(window));
    win_ptr->cam.zoom -= (float)yoffset;
    if (win_ptr->cam.zoom < 1.0f)
        win_ptr->cam.zoom = 1.0f;
    if (win_ptr->cam.zoom > 45.0f)
        win_ptr->cam.zoom = 45.0f;
    win_ptr->camZoom = win_ptr->cam.zoom;
}

void Window7::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window7* win_ptr = static_cast<Window7*>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            win_ptr->keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            win_ptr->keys[key] = false;
        }
    }
}

int Window7::getWidth()
{
    return width;
}

int Window7::getHeight()
{
    return height;
}

inline void Window7::setSize(int n_width, int n_height)
{
    width = n_width;
    height = n_height;
}
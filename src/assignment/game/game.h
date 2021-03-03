#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include "gui.h"
#include "terrain.h"
#include "lamp.h"
#include "orb.h"
#include "ghost.h"
#include "player.h"
#include "shrine.h"
#include "fence.h"
#include "skybox.h"
#include <iostream>

//define methods
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
GLFWwindow* initialiseOpenGl();
void loadTextures();
void setBlockVertex();
    

#endif
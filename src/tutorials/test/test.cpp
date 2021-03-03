#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>

#include "terrain_element.h"


#include <iostream>

void generateMap(terrain_element** terrain_elements);
//void generateMap(glm::vec3** positions);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(100.0f, 5.0f, 100.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("shader.vs", "shader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    int rows = 200;
    int cols = 200;

    /* glm::vec3** positions = (glm::vec3**)malloc(rows * sizeof(glm::vec3*));
    for(int i = 0; i < rows; i++)
    {
        positions[i] = (glm::vec3*)malloc(cols * sizeof(glm::vec3));
    } */

    terrain_element** terrain_elements = (terrain_element**)malloc(rows * sizeof(terrain_element*));
    for(int i = 0; i < rows; i++)
    {
        terrain_elements[i] = (terrain_element*)malloc(cols * sizeof(terrain_element));
        //(glm::vec3*)malloc(cols * sizeof(glm::vec3));
    }

    glm::vec3 tree_block_positions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 2.0f, 0.0f),
        glm::vec3(0.0f, 3.0f, 0.0f),
        glm::vec3(0.0f, 4.0f, 0.0f),
        glm::vec3(0.0f, 5.0f, 0.0f)
    };

    generateMap(terrain_elements);


    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // load and create a texture 
    // -------------------------
    unsigned int debug_0, dirt_0, dirt_1, grass_0, grass_1, stone_0, stone_1;
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // texture 1
    // ---------
    glGenTextures(1, &grass_0);
    glBindTexture(GL_TEXTURE_2D, grass_0);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // load image, create texture and generate mipmaps
    unsigned char* grass_0_data = stbi_load(FileSystem::getPath("resources/textures/grass_0.png").c_str(), &width, &height, &nrChannels, 0);
    if (grass_0_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, grass_0_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(grass_0_data);


    glGenTextures(1, &grass_1);
    glBindTexture(GL_TEXTURE_2D, grass_1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps
    unsigned char* grass_1_data = stbi_load(FileSystem::getPath("resources/textures/grass_1.png").c_str(), &width, &height, &nrChannels, 0);
    if (grass_1_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, grass_1_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(grass_1_data);


    // texture 2
    // ---------
    glGenTextures(1, &dirt_0);
    glBindTexture(GL_TEXTURE_2D, dirt_0);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* dirt_0_data = stbi_load(FileSystem::getPath("resources/textures/dirt_0.png").c_str(), &width, &height, &nrChannels, 0);
    if (dirt_0_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dirt_0_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(dirt_0_data);


    glGenTextures(1, &dirt_1);
    glBindTexture(GL_TEXTURE_2D, dirt_1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* dirt_1_data = stbi_load(FileSystem::getPath("resources/textures/dirt_1.png").c_str(), &width, &height, &nrChannels, 0);
    if (dirt_1_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dirt_1_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(dirt_1_data);

    glGenTextures(1, &stone_0);
    glBindTexture(GL_TEXTURE_2D, stone_0);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* stone_0_data = stbi_load(FileSystem::getPath("resources/textures/stone_0.png").c_str(), &width, &height, &nrChannels, 0);
    if (stone_0_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, stone_0_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(stone_0_data);

    glGenTextures(1, &stone_1);
    glBindTexture(GL_TEXTURE_2D, stone_1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* stone_1_data = stbi_load(FileSystem::getPath("resources/textures/stone_1.png").c_str(), &width, &height, &nrChannels, 0);
    if (stone_1_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, stone_1_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(stone_1_data);
    
    
    

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);
    //ourShader.setInt("texture2", 1);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grass_0);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader
        ourShader.use();

        // create transformations
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);

        // render box
        glBindVertexArray(VAO);

        int cam_x = camera.GetPosition().x;
        int cam_z = camera.GetPosition().z;

        int render_distance = 50;

        for(int i = (cam_x) - render_distance; i < ((cam_x + render_distance)); i++)
        {
            for(int j = (cam_z) - render_distance; j < ((cam_z + render_distance)); j++)
            {
                    glm::mat4 model = glm::mat4(1.0f);
                    if(i < 200 && i >= 0 && j < 200 && j >= 0)
                    {
                        glm::vec3 block_position = glm::vec3((terrain_elements[i][j].position->x), terrain_elements[i][j].position->y, (terrain_elements[i][j].position->z));
                        glm::mat4 model = glm::mat4(1.0f);
                        model = glm::translate(model, block_position);

                        
                        //draw more under so there aren't holes
                        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
                        ourShader.setMat4("model", model);
                        glDrawArrays(GL_TRIANGLES, 0, 36);

                        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
                        ourShader.setMat4("model", model);
                        glDrawArrays(GL_TRIANGLES, 0, 36);

                        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
                        ourShader.setMat4("model", model);
                        glDrawArrays(GL_TRIANGLES, 0, 36);

                        model = glm::mat4(1.0f);
                        model = glm::translate(model, block_position);
                        
                        if(strcmp(terrain_elements[i][j].texture_name, "grass_0") == 0)
                        {
                            glActiveTexture(GL_TEXTURE0);
                            glBindTexture(GL_TEXTURE_2D, grass_0);
                            glDrawArrays(GL_TRIANGLES, 0, 36);

                            if(terrain_elements[i][j].has_tree == true)
                            {
                                for(int k = 0; k < (int)(sizeof(tree_block_positions)/sizeof(glm::vec3)); k++)
                                {
                                    glm::vec3 block_pos = tree_block_positions[k];
                                    glm::vec3 tree_position = glm::vec3((terrain_elements[i][j].position->x), terrain_elements[i][j].position->y, (terrain_elements[i][j].position->z));
                                    model = glm::mat4(1.0f);
                                    model = glm::translate(model, block_pos);
                                    model = glm::translate(model, tree_position);

                                    model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
                                    ourShader.setMat4("model", model);

                                    glActiveTexture(GL_TEXTURE0);
                                    glBindTexture(GL_TEXTURE_2D, dirt_0);
                                
                                    glDrawArrays(GL_TRIANGLES, 0, 36);
                                }    
                            }

                        }
                        else if(strcmp(terrain_elements[i][j].texture_name, "grass_1") == 0)
                        {
                            glActiveTexture(GL_TEXTURE0);
                            glBindTexture(GL_TEXTURE_2D, grass_1);
                        }
                        else if(strcmp(terrain_elements[i][j].texture_name, "dirt_0") == 0)
                        {
                            glActiveTexture(GL_TEXTURE0);
                            glBindTexture(GL_TEXTURE_2D, dirt_0);
                        }     
                        else if(strcmp(terrain_elements[i][j].texture_name, "dirt_1") == 0)
                        {
                            glActiveTexture(GL_TEXTURE0);
                            glBindTexture(GL_TEXTURE_2D, dirt_1);
                        } 
                        else if(strcmp(terrain_elements[i][j].texture_name, "stone_0") == 0)
                        {
                            glActiveTexture(GL_TEXTURE0);
                            glBindTexture(GL_TEXTURE_2D, stone_0);
                        }
                        else if(strcmp(terrain_elements[i][j].texture_name, "stone_1") == 0)
                        {
                            glActiveTexture(GL_TEXTURE0);
                            glBindTexture(GL_TEXTURE_2D, stone_1);
                        }
                        else
                        {
                            glActiveTexture(GL_TEXTURE0);
                            glBindTexture(GL_TEXTURE_2D, dirt_0);
                        } 
                        ourShader.setMat4("model", model);
                        glDrawArrays(GL_TRIANGLES, 0, 36);
                        //std::cout << terrain_elements[i][j].has_tree;

                        
 
                        //glActiveTexture(GL_TEXTURE0);
                        //glBindTexture(GL_TEXTURE_2D, stone_0);

                        
                    }    
            }

            glm::vec3 cam_pos = camera.GetPosition();
            glm::vec3 block_pos = *terrain_elements[(int)round(cam_pos.x)][(int)round(cam_pos.z)].position;
            //still need to check if camera is within world range

            camera.SetYPosition(block_pos.y + 2.5f);
            /* if(block_pos.y <= cam_pos.y - 2.5f)
            {
                camera.SetYPosition(block_pos.y + 2.5f);
            }
            else
            {
                camera.SetPosition(camera.GetPosition());
            } */
            

        }

        //printf("\n(%d, %d)\n", (int)camera.GetPosition().x, (int)camera.GetPosition().z);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void generateMap(terrain_element** terrain_elements)
{
    int r, b, g;
    int pixel_brightness;
    int width, height, channels;

    unsigned char* height_map = stbi_load(FileSystem::getPath("resources/textures/height_map.jpg").c_str(), &width, &height, &channels, 0);
    if(height_map == NULL)
    {
        printf("Error in loading the image\n");
        exit(1);
    }
    
    //unsigned char height_map[1000][1000];
    int brightness_map[1000][1000];
    int total_brightness = 0;
    int average_brightness;
    for(int i = 0; i < 1000 * 1000 * 3; i += 3)
    {
        r = (int)(height_map[i]);
        b = (int)(height_map[i+1]);
        g = (int)(height_map[i+2]);
        pixel_brightness = pow(((r + b + g) / 3), 1.02);

        int row = ((i/3) % 1000);
        int col = ((i/3) / 1000);

        brightness_map[row][col] = pixel_brightness;
        total_brightness += pixel_brightness;
    }

    average_brightness = total_brightness / (1000 * 1000);


    if(height_map != NULL && width > 0 && height > 0)
    {
        int start_row = rand() % 799;
        int start_col = rand() % 799;
        
        for(int i = start_row; i < (start_row + 200); i++)
        {
            for(int j = start_col; j < (start_col + 200); j++)
            {
                terrain_elements[i - start_row][j - start_col].position = new glm::vec3((float)(i - start_row), (float)(brightness_map[i][j] - average_brightness), (float)(j - start_col));

                if(brightness_map[i][j] - average_brightness < -3)
                {
                    terrain_elements[i - start_row][j - start_col].texture_name = "dirt_1";
                    terrain_elements[i - start_row][j - start_col].has_tree = false;
                }
                else if(brightness_map[i][j] - average_brightness < 0)
                {
                    terrain_elements[i - start_row][j - start_col].texture_name = "dirt_0";
                    terrain_elements[i - start_row][j - start_col].has_tree = false;
                }
                else if(brightness_map[i][j] - average_brightness < 3)
                {
                    terrain_elements[i - start_row][j - start_col].texture_name = "grass_0";
                    if((float)rand()/RAND_MAX < 0.02f)
                    {
                        terrain_elements[i - start_row][j - start_col].has_tree = true;
                    }
                    else
                    {
                        terrain_elements[i - start_row][j - start_col].has_tree = false;
                    }
                }
                else if(brightness_map[i][j] - average_brightness < 5)
                {
                    terrain_elements[i - start_row][j - start_col].texture_name = "stone_0";
                    terrain_elements[i - start_row][j - start_col].has_tree = false;
                }
                else
                {
                    terrain_elements[i - start_row][j - start_col].texture_name = "stone_1";
                    terrain_elements[i - start_row][j - start_col].has_tree = false;
                }
            }
        }
                

    }
    else
    {
        printf("Error in loading the image\n");
        exit(1);
    }

    stbi_image_free(height_map);
}



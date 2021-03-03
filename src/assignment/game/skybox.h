#ifndef SKYBOX_H
#define SKYBOX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "block.h"

//basically just a big textured cube to represent the sky
class SkyBox
{
    private:
        glm::vec3 skyblock_block_position;
        int num_blocks = 1;
        bool is_day = false;

    public:
        SkyBox()
        {
            glm::vec3 skyblock_block_position_(0.0f, 0.0f, 0.0f);
    

            skyblock_block_position = skyblock_block_position_;
        }

        void draw(Shader* shader, glm::vec3 position)
        {
            Block* block;

            //draw skyblock with different texture depending on day or night
            if(!is_day)
            {
                block = new Block(11, (skyblock_block_position + position), glm::vec3(200.0f, 200.0f, 200.0f));
            }
            else
            {
                block = new Block(16, (skyblock_block_position + position), glm::vec3(200.0f, 200.0f, 200.0f));
            }
            
            block->draw(shader);

            //free memory
            delete(block);
        }

        void setToDay()
        {
            is_day = true;
        }

        void setToNight()
        {
            is_day = false;
        }
};
    

#endif
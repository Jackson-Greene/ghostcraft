#ifndef LAMP_H
#define LAMP_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "block.h"


//the light source in the game is lamp
//lighting is dealt with elsewhere this class
//allows drawing of the lamp
class Lamp
{
    private:
        glm::vec3* lamp_block_positions;
        glm::vec3* lamp_block_scales;
        int num_blocks = 8;

    public:
        Lamp()
        {
            //positions for each block part of the lamp
            glm::vec3 lamp_block_positions_[] = 
            {
                glm::vec3(0.0f, 0.0f, 0.0f),

                glm::vec3(0.1f, 0.0f, 0.1f),
                glm::vec3(0.1f, 0.0f, -0.1f),
                glm::vec3(-0.1f, 0.0f, 0.1f),
                glm::vec3(-0.1f, 0.0f, -0.1f),

                glm::vec3(0.0f, -0.2f, 0.0f),
                glm::vec3(0.0f, 0.2f, 0.0f),

                glm::vec3(0.0f, 0.25f, 0.0f),
            };

            lamp_block_positions = (glm::vec3*)malloc(sizeof(glm::vec3) * num_blocks);

            for(int i = 0; i < num_blocks; i++)
            {
                lamp_block_positions[i] = lamp_block_positions_[i];
            } 

            //scales for each part of the lamp
            glm::vec3 lamp_block_scales_[] = 
            {
                glm::vec3(0.2f, 0.4f, 0.2f),

                glm::vec3(0.05f, 0.4f, 0.05f),
                glm::vec3(0.05f, 0.4f, 0.05f),
                glm::vec3(0.05f, 0.4f, 0.05f),
                glm::vec3(0.05f, 0.4f, 0.05f),

                glm::vec3(0.30f, 0.05f, 0.30f),
                glm::vec3(0.30f, 0.05f, 0.30f),

                glm::vec3(0.2f, 0.05f, 0.2f),
            };

            lamp_block_scales = (glm::vec3*)malloc(sizeof(glm::vec3) * num_blocks);

            for(int i = 0; i < num_blocks; i++)
            {
                lamp_block_scales[i] = lamp_block_scales_[i];
            }
        }


        //deconstructor
        ~Lamp()
        {
            free(lamp_block_positions);
            free(lamp_block_scales);
        }


        void draw(Shader* shader, glm::vec3 position, float rotation, glm::vec3 rotation_axis)
        {
            Block** blocks = (Block**)malloc(sizeof(Block*) * num_blocks);
           
            //first block is the yellow glass / light part
            blocks[0] = new Block(12, (lamp_block_positions[0] + position), lamp_block_scales[0], rotation, rotation_axis);

            //other blocks are metal texture
            for(int i = 1; i < num_blocks; i++)
            {
                blocks[i] = new Block(13, (lamp_block_positions[i] + position), lamp_block_scales[i], rotation, rotation_axis);
            }

            //draw all blocks
            for(int i = 0; i < num_blocks; i++)
            {
                blocks[i]->drawWithRotationOffset(shader, lamp_block_positions[i]);
            }

            //free blocks and pointer itself
            for(int i = 0; i < num_blocks; i++)
            {
                delete(blocks[i]);
            }

            free(blocks);
        }
};
    

#endif
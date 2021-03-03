#ifndef SHRINE_H
#define SHRINE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "block.h"

class Shrine
{
    private:
        glm::vec3* shrine_block_positions;
        glm::vec3* shrine_block_scales;
        int num_blocks = 12;

    public:
        Shrine()
        {
            //positions of shrine blocks
            glm::vec3 shrine_block_positions_[] = 
            {
                glm::vec3(0.0f, 1.0f, 0.0f),

                glm::vec3(0.0f, 1.2f, 0.0f),
                glm::vec3(0.0f, 1.4f, 0.0f),
                glm::vec3(0.0f, 2.5f, 0.0f),

                glm::vec3(2.0f, 1.0f, 0.0f),
                glm::vec3(-2.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 2.0f),
                glm::vec3(0.0f, 1.0f, -2.0f),

                glm::vec3(3.0f, 1.0f, 0.0f),
                glm::vec3(-3.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 3.0f),
                glm::vec3(0.0f, 1.0f, -3.0f),
            };

            shrine_block_positions = (glm::vec3*)malloc(sizeof(glm::vec3) * num_blocks);

            for(int i = 0; i < num_blocks; i++)
            {
                shrine_block_positions[i] = shrine_block_positions_[i];
            } 

            //scales of shrine blocks
            glm::vec3 shrine_block_scales_[] = 
            {
                glm::vec3(2.0f, 0.2f, 2.0f),
                
                glm::vec3(1.0f, 0.2f, 1.0f),
                glm::vec3(0.5f, 0.2f, 0.5f),
                glm::vec3(0.8f, 0.8f, 0.8f),

                glm::vec3(0.5f, 5.0f, 0.5f),
                glm::vec3(0.5f, 5.0f, 0.5f),
                glm::vec3(0.5f, 5.0f, 0.5f),
                glm::vec3(0.5f, 5.0f, 0.5f),

                glm::vec3(0.5f, 3.0f, 0.5f),
                glm::vec3(0.5f, 3.0f, 0.5f),
                glm::vec3(0.5f, 3.0f, 0.5f),
                glm::vec3(0.5f, 3.0f, 0.5f),
            };

            shrine_block_scales = (glm::vec3*)malloc(sizeof(glm::vec3) * num_blocks);

            for(int i = 0; i < num_blocks; i++)
            {
                shrine_block_scales[i] = shrine_block_scales_[i];
            }
        }


        //deconstructor
        ~Shrine()
        {
            free(shrine_block_positions);
            free(shrine_block_scales);
        }


        void draw(Shader* shader, glm::vec3 position)
        {
            Block** blocks = (Block**)malloc(sizeof(Block*) * num_blocks);

            for(int i = 0; i < 3; i++)
            {
                blocks[i] = new Block(13, (shrine_block_positions[i] + position), shrine_block_scales[i]);
            }

            //make the middle block rotate over time, no need to worry about delta time 
            //since it doesn't really matter if it runs faster on other system
            //looks smoother without delta time (float precision?)
            float rotation = 50.0f * glfwGetTime();
            blocks[3] = new Block(15, (shrine_block_positions[3] + position), shrine_block_scales[3], rotation, glm::vec3(1.0f, 1.0f, 1.0f));

            for(int i = 4; i < num_blocks; i++)
            {
                blocks[i] = new Block(9, (shrine_block_positions[i] + position), shrine_block_scales[i]);
            }

            for(int i = 0; i < num_blocks; i++)
            {
                blocks[i]->draw(shader);
            }

            //free blocks and pointer itself
            for(int i = 0; i < num_blocks; i++)
            {
                delete(blocks[i]);
            }

            free(blocks);
        }


        void drawActivated(Shader* shader, glm::vec3 position)
        {
            Block** blocks = (Block**)malloc(sizeof(Block*) * num_blocks);

            //make middle part of shrine move up and down over time (using offset where y changes with sin value)
            glm::vec3 pos_offset = glm::vec3(0.0f, sin(glfwGetTime()) + 1.5f, 0.0f);
            float rotation = glfwGetTime() * 250.0f;

            for(int i = 0; i < 3; i++)
            {
                blocks[i] = new Block(13, (shrine_block_positions[i] + position + pos_offset), shrine_block_scales[i]);
            }
            
            //make the middle block rotate faster than before over time, no need to worry about delta time 
            //since it doesn't really matter if it runs faster on other system
            //looks smoother without delta time (float precision?)
            blocks[3] = new Block(15, (shrine_block_positions[3] + position + pos_offset), shrine_block_scales[3], rotation , glm::vec3(1.0f, 1.0f, 1.0f));

            for(int i = 4; i < num_blocks; i++)
            {
                blocks[i] = new Block(9, (shrine_block_positions[i] + position), shrine_block_scales[i]);
            }

            for(int i = 0; i < num_blocks; i++)
            {
                blocks[i]->draw(shader);
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
#ifndef GHOST_H
#define GHOST_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "block.h"

//the ghost is the enemy in the game
//this class just deals with how to draw it
//player following logic is elsewhere
class Ghost
{
    private:
        glm::vec3* ghost_block_positions;
        glm::vec3* ghost_block_scales;
        int num_blocks = 11;

    public:
        Ghost()
        {
            //block positions that make the ghost with arms and the mini ghosts
            glm::vec3 ghost_block_positions_[] = 
            {
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.5f, -0.2f, 0.0f),
                glm::vec3(-0.5f, -0.2f, 0.0f),
                
                glm::vec3(1.0f, -1.0f, 0.1f),
                glm::vec3(1.0f, -1.0f, 0.9f),

                glm::vec3(1.0f, -1.0f, -0.1f),
                glm::vec3(1.0f, -1.0f, -0.9f),

                glm::vec3(-1.0f, -1.0f, 0.1f),
                glm::vec3(-1.0f, -1.0f, 0.9f),

                glm::vec3(-1.0f, -1.0f, -0.1f),
                glm::vec3(-1.0f, -1.0f, -0.9f),
            };
            

            ghost_block_positions = (glm::vec3*)malloc(sizeof(glm::vec3) * num_blocks);

            for(int i = 0; i < num_blocks; i++)
            {
                ghost_block_positions[i] = ghost_block_positions_[i];
            }

            //scales for each block
            glm::vec3 ghost_block_scales_[] = 
            {
                glm::vec3(1.0f, 3.0f, 1.0f),
                glm::vec3(2.0f, 0.4f, 0.4f),
                glm::vec3(2.0f, 0.4f, 0.4f),
                
                glm::vec3(0.5f, 1.0f, 0.5f),
                glm::vec3(0.5f, 1.0f, 0.5f),

                glm::vec3(0.5f, 1.0f, 0.5f),
                glm::vec3(0.5f, 1.0f, 0.5f),

                glm::vec3(0.5f, 1.0f, 0.5f),
                glm::vec3(0.5f, 1.0f, 0.5f),

                glm::vec3(0.5f, 1.0f, 0.5f),
                glm::vec3(0.5f, 1.0f, 0.5f),
            };
            

            ghost_block_scales = (glm::vec3*)malloc(sizeof(glm::vec3) * num_blocks);

            for(int i = 0; i < num_blocks; i++)
            {
                ghost_block_scales[i] = ghost_block_scales_[i];
            }
        }


        //deconstructor
        ~Ghost()
        {
            free(ghost_block_positions);
            free(ghost_block_scales);
        }


        void draw(Shader* shader, glm::vec3 position, float rotation)
        {
            Block** blocks = (Block**)malloc(sizeof(Block*) * num_blocks);

            blocks[0] = new Block(8, ghost_block_positions[0] + position, ghost_block_scales[0], rotation, glm::vec3(0.0f, 1.0f, 0.0f));
            blocks[1] = new Block(9, ghost_block_positions[1] + position, ghost_block_scales[1], rotation, glm::vec3(0.0f, 1.0f, 0.0f));
            blocks[2] = new Block(9, ghost_block_positions[2] + position, ghost_block_scales[2], rotation, glm::vec3(0.0f, 1.0f, 0.0f));

            float rand_num;

            for(int i = 3; i < num_blocks; i++)
            {         
                //random position offset for the small ghosts        
                srand((int)glfwGetTime() + i);
                rand_num = 2 * (float)rand() / (float)RAND_MAX;
                if(i % 2 == 0)
                {
                    rand_num = rand_num * -1;
                }

                //small ghosts have random position so they look like they are moving around
                glm::vec3 small_ghost_pos = glm::vec3(ghost_block_positions[i] + position + glm::vec3(rand_num, rand_num * 0.1, rand_num));
                
                blocks[i] = new Block(8, small_ghost_pos, ghost_block_scales[i], rotation, glm::vec3(0.0f, 1.0f, 0.0f));
            } 
            
            //draw big ghost, arms need to rotate properly so draw with offset
            blocks[0]->draw(shader);
            blocks[1]->drawWithRotationOffset(shader, ghost_block_positions[1]);
            blocks[2]->drawWithRotationOffset(shader, ghost_block_positions[2]);

            //draw all blocks
            for(int i = 3; i < num_blocks; i++)
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
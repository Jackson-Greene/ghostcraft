#ifndef FENCE_H
#define FENCE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "block.h"

class Fence
{
    private:
        glm::vec3* fence_block_positions;
        glm::vec3* fence_block_scales;
        int num_blocks = 13;

    public:
        Fence()
        {
            //positions of fence blocks relative to each other
            glm::vec3 fence_block_positions_[] = 
            {
                glm::vec3(0.5f, 0.0f, 0.0f),
                glm::vec3(-0.5f, 0.0f, 0.0f),

                glm::vec3(0.0f, 0.25f, 0.0f),
                glm::vec3(0.0f, 0.5f, 0.0f),
                glm::vec3(0.0f, 0.75f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 1.25f, 0.0f),

                glm::vec3(0.0f, 0.0f, 0.0f),

                glm::vec3(0.0f, -0.25f, 0.0f),
                glm::vec3(0.0f, -0.5f, 0.0f),
                glm::vec3(0.0f, -0.75f, 0.0f),
                glm::vec3(0.0f, -1.0f, 0.0f),
                glm::vec3(0.0f, -1.25f, 0.0f),
            };

            fence_block_positions = (glm::vec3*)malloc(sizeof(glm::vec3) * num_blocks);

            for(int i = 0; i < num_blocks; i++)
            {
                fence_block_positions[i] = fence_block_positions_[i];
            } 

            //scale of each block at the positions described earlier
            glm::vec3 fence_block_scales_[] = 
            {
                //glm::vec3(0.0f, 0.0f, 0.5f),
                glm::vec3(0.1f, 3.0f, 0.1f),
                glm::vec3(0.1f, 3.0f, 0.1f),

                glm::vec3(1.0f, 0.15f, 0.1f),
                glm::vec3(1.0f, 0.15f, 0.1f),
                glm::vec3(1.0f, 0.15f, 0.1f),
                glm::vec3(1.0f, 0.15f, 0.1f),
                glm::vec3(1.0f, 0.15f, 0.1f),

                glm::vec3(1.0f, 0.15f, 0.1f),

                glm::vec3(1.0f, 0.15f, 0.1f),
                glm::vec3(1.0f, 0.15f, 0.1f),
                glm::vec3(1.0f, 0.15f, 0.1f),
                glm::vec3(1.0f, 0.15f, 0.1f),
                glm::vec3(1.0f, 0.15f, 0.1f),
            };

            fence_block_scales = (glm::vec3*)malloc(sizeof(glm::vec3) * num_blocks);

            for(int i = 0; i < num_blocks; i++)
            {
                fence_block_scales[i] = fence_block_scales_[i];
            }
        }


        //deconstructor
        ~Fence()
        {
            free(fence_block_positions);
            free(fence_block_scales);
        }


        void draw(Shader* shader, glm::vec3 position, float rotation, glm::vec3 rotation_axis)
        {
            Block** blocks = (Block**)malloc(sizeof(Block*) * num_blocks);
           
            //all blocks created at specified position and rotation with metal texture
            for(int i = 0; i < num_blocks; i++)
            {
                blocks[i] = new Block(13, (fence_block_positions[i] + position), fence_block_scales[i], rotation, rotation_axis);
            }
            
            //draw all blocks
            for(int i = 0; i < num_blocks; i++)
            {
                blocks[i]->drawWithRotationOffset(shader, fence_block_positions[i]);
            }

            //free blocks and pointer itself
            for(int i = 0; i < num_blocks; i++)
            {
                delete(blocks[i]);
            }

            free(blocks);
        }


        void draw(Shader* shader, glm::vec3 position)
        {
            Block** blocks = (Block**)malloc(sizeof(Block*) * num_blocks);
            
            //all blocks created at specified position and rotation with metal texture
            for(int i = 0; i < num_blocks; i++)
            {
                blocks[i] = new Block(13, (fence_block_positions[i] + position), fence_block_scales[i]);
            }
            
            //draw all blocks
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
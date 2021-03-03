#ifndef ORB_H
#define ORB_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "block.h"

class Orb
{
    private:
        glm::vec3* orb_block_positions;
        glm::vec3* orb_block_scales;
        int num_blocks = 7;

    public:
        Orb()
        {
            //orb is made up of multiple blocks to look more circular
            glm::vec3 orb_block_positions_[] = 
            {
                glm::vec3(0.0f, 0.0f, 0.0f),

                glm::vec3(0.2f, 0.0f, 0.0f),
                glm::vec3(-0.2f, 0.0f, 0.0f),

                glm::vec3(0.0f, 0.0f, 0.2f),
                glm::vec3(0.0f, 0.0f, -0.2f),

                glm::vec3(0.0f, 0.2f, 0.0f),
                glm::vec3(0.0f, -0.2f, 0.0f),
            };

            orb_block_positions = (glm::vec3*)malloc(sizeof(glm::vec3) * num_blocks);

            for(int i = 0; i < num_blocks; i++)
            {
                orb_block_positions[i] = orb_block_positions_[i];
            } 


            glm::vec3 orb_block_scales_[] = 
            {
                glm::vec3(0.4f, 0.4f, 0.4f),

                glm::vec3(0.1f, 0.3f, 0.3f),
                glm::vec3(0.1f, 0.3f, 0.3f),

                glm::vec3(0.3f, 0.3f, 0.1f),
                glm::vec3(0.3f, 0.3f, 0.1f),

                glm::vec3(0.3f, 0.1f, 0.3f),
                glm::vec3(0.3f, 0.1f, 0.3f),
            };

            orb_block_scales = (glm::vec3*)malloc(sizeof(glm::vec3) * num_blocks);

            for(int i = 0; i < num_blocks; i++)
            {
                orb_block_scales[i] = orb_block_scales_[i];
            }
        }


        //deconstructor
        ~Orb()
        {
            free(orb_block_positions);
            free(orb_block_scales);
        }



        void draw(Shader* shader, glm::vec3 position, float rotation, glm::vec3 rotation_axis)
        {
            Block** blocks = (Block**)malloc(sizeof(Block*) * num_blocks);
           
            for(int i = 0; i < num_blocks; i++)
            {
                blocks[i] = new Block(14, (orb_block_positions[i] + position), orb_block_scales[i], rotation, rotation_axis);
            }

            blocks[0]->draw(shader);

            //draw the non center blocks with rotation offset so they rotate correctly
            for(int i = 1; i < num_blocks; i++)
            {
                blocks[i]->drawWithRotationOffset(shader, orb_block_positions[i]);
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
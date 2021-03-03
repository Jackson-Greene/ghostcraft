#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "block.h"

class Player
{
    private:
        glm::vec3* player_block_positions;
        int num_blocks = 3;
        

    public:
        Player()
        {
            //player arm positions
            glm::vec3 player_block_positions_[] = 
            {
                glm::vec3(0.5f, 0.0f, 0.25f),
                glm::vec3(-0.5f, 0.0f, 0.25f),

                glm::vec3(0.0f, 0.0f, 1.5f),
            };

            
            player_block_positions = (glm::vec3*)malloc(sizeof(glm::vec3) * num_blocks);

            for(int i = 0; i < num_blocks; i++)
            {
                player_block_positions[i] = player_block_positions_[i];
            } 
        }


        //deconstructor
        ~Player()
        {
            free(player_block_positions);
        }


        void draw(Shader* shader, glm::vec3 position, float rotation, glm::vec3 rotation_axis)
        {
            Block** blocks = (Block**)malloc(sizeof(Block*) * num_blocks);
            //glm::vec3 arm_scale(0.2f, 0.2f, 0.65f);
            glm::vec3 arm_scale(0.2f, 0.2f, 2.0f);
            glm::vec3 body_scale(1.5f, 2.0f, 1.5f);


            for(int i = 0; i < 2; i++)
            {
                blocks[i] = new Block(10, (player_block_positions[i] + position), arm_scale, rotation, rotation_axis);
            }

            blocks[2] = new Block(10, (player_block_positions[2] + position), body_scale, rotation, rotation_axis);

    
            //draw arms infront of player, they are rotated with offset as they aren't at 0, 0, 0 locally
            for(int i = 0; i < num_blocks; i++)
            {
                blocks[i]->drawWithRotationOffset(shader, player_block_positions[i]);
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
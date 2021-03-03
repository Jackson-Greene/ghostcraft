#ifndef TREE_H
#define TREE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "block.h"

class Tree
{
    private:
        glm::vec3* tree_block_positions;
        int num_blocks = 51;
        //no need for scales and rotations

    public:
        Tree()
        {
            glm::vec3 tree_block_positions_[] = 
            {
                //wood
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 2.0f, 0.0f),
                glm::vec3(0.0f, 3.0f, 0.0f),
                glm::vec3(0.0f, 4.0f, 0.0f), 
                glm::vec3(0.0f, 5.0f, 0.0f),
                glm::vec3(0.0f, 6.0f, 0.0f), 
                glm::vec3(0.0f, 7.0f, 0.0f),
                glm::vec3(0.0f, 8.0f, 0.0f),
                glm::vec3(0.0f, 9.0f, 0.0f),

                //leaves 
                glm::vec3(0.0f, 10.0f, 0.0f),
                glm::vec3(1.0f, 9.0f, 0.0f),
                glm::vec3(-1.0f, 9.0f, 0.0f),
                glm::vec3(0.0f, 9.0f, 1.0f),
                glm::vec3(0.0f, 9.0f, -1.0f),

                glm::vec3(1.0f, 8.0f, 0.0f),
                glm::vec3(-1.0f, 8.0f, 0.0f),
                glm::vec3(0.0f, 8.0f, 1.0f),
                glm::vec3(0.0f, 8.0f, -1.0f),
                glm::vec3(2.0f, 8.0f, 0.0f),
                glm::vec3(-2.0f, 8.0f, 0.0f),
                glm::vec3(0.0f, 8.0f, 2.0f),
                glm::vec3(0.0f, 8.0f, -2.0f),
                glm::vec3(1.0f, 8.0f, 1.0f),
                glm::vec3(1.0f, 8.0f, -1.0f),
                glm::vec3(-1.0f, 8.0f, 1.0f),
                glm::vec3(-1.0f, 8.0f, -1.0f),

                glm::vec3(1.0f, 6.0f, 0.0f),
                glm::vec3(-1.0f, 6.0f, 0.0f),
                glm::vec3(0.0f, 6.0f, 1.0f),
                glm::vec3(0.0f, 6.0f, -1.0f),
                glm::vec3(2.0f, 6.0f, 0.0f),
                glm::vec3(-2.0f, 6.0f, 0.0f),
                glm::vec3(0.0f, 6.0f, 2.0f),
                glm::vec3(0.0f, 6.0f, -2.0f),

                glm::vec3(1.0f, 5.0f, 0.0f),
                glm::vec3(-1.0f, 5.0f, 0.0f),
                glm::vec3(0.0f, 5.0f, 1.0f),
                glm::vec3(0.0f, 5.0f, -1.0f),
                glm::vec3(2.0f, 5.0f, 0.0f),
                glm::vec3(-2.0f, 5.0f, 0.0f),
                glm::vec3(0.0f, 5.0f, 2.0f),
                glm::vec3(0.0f, 5.0f, -2.0f),
                glm::vec3(1.0f, 5.0f, 1.0f),
                glm::vec3(1.0f, 5.0f, -1.0f),
                glm::vec3(-1.0f, 5.0f, 1.0f),
                glm::vec3(-1.0f, 5.0f, -1.0f),
                glm::vec3(3.0f, 5.0f, 0.0f),
                glm::vec3(-3.0f, 5.0f, 0.0f),
                glm::vec3(0.0f, 5.0f, 3.0f),
                glm::vec3(0.0f, 5.0f, -3.0f),
            };

            tree_block_positions = (glm::vec3*)malloc(sizeof(glm::vec3) * num_blocks);

            for(int i = 0; i < num_blocks; i++)
            {
                tree_block_positions[i] = tree_block_positions_[i];
            } 
        }


        //deconstructor
        ~Tree()
        {
            free(tree_block_positions);
        }


        void draw(Shader* shader, glm::vec3 position)
        {
            Block** blocks = (Block**)malloc(sizeof(Block*) * num_blocks);
            
            //first blocks are wood texture
            for(int i = 0; i < 9; i++)
            {
                blocks[i] = new Block(5, (tree_block_positions[i] + position));
            }

            //last blocks are leaf texture
            for(int i = 9; i < num_blocks; i++)
            {
                blocks[i] = new Block(6, (tree_block_positions[i] + position));
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
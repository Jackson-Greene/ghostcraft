#ifndef GUI_H
#define GUI_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "block.h"


//a gui (in this case not really an interface because user can't interact) is just a texture applied
//to a cube.. then it can be placed infront of the camera
class Gui
{
    private:
        glm::vec3 gui_block_position;
        int num_blocks = 1;
        int texture_id;

    public:
        Gui(int texture_id_)
        {
            texture_id = texture_id_;

            glm::vec3 gui_block_position_(0.0f, 0.0f, 0.0f);

            gui_block_position = gui_block_position_;
        }

        void draw(Shader* shader, glm::vec3 position, glm::vec3 scale, float rotation)
        {
            Block* block;

            block = new Block(texture_id, (gui_block_position + position), scale, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
            
            block->draw(shader);

            //free block
            delete(block);
        }

        void setTextureId(int texture_id_)
        {
            texture_id = texture_id_;
        }
};
    

#endif
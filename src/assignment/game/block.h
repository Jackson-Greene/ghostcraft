#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader.h>
#include "texture_library.h"

extern TextureLibrary* texture_library;

//a block is a single cube with a position, scale, rotation and texture
//everything is made from blocks and each block calls it's draw to build everything
class Block
{
    private:
        int id;
        //world position rotation and scale
        glm::vec3 position;
        glm::vec3 scale;
        float rotation;
        glm::vec3 rotation_axis;

        bool is_rotation;
    
    public:

        //different constructors because sometimes rotation and scale doesn't need to change and it'd be
        //annoying to specify each call to create a block

        Block(int id_, glm::vec3 position_, glm::vec3 scale_, float rotation_, glm::vec3 rotation_axis_)
        {
            id = id_;
            position = position_;
            scale = scale_;
            rotation = rotation_;
            rotation_axis = rotation_axis_;
            is_rotation = true;
        }

        Block(int id_, glm::vec3 position_, glm::vec3 scale_)
        {
            id = id_;
            position = position_;
            scale = scale_;
            is_rotation = false;
        }

        Block(int id_, glm::vec3 position_)
        {
            id = id_;
            position = position_;
            scale = glm::vec3(1.0f, 1.0f, 1.0f);
            is_rotation = false;
        }
        

        //get the position of a block in the world
        glm::vec3 getPosition()
        {
            return(position);
        }


        //draw a block
        void draw(Shader* shader)
        {
            //do matrix operations to generate view matrix
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            if(is_rotation)
            {
                model = glm::rotate(model, glm::radians(rotation), rotation_axis);
            }
            model = glm::scale(model, scale);
            
            //each shader has "model"
            shader->setMat4("model", model);

            //first texture is diffuse
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_library->getDiffuse(id));

            //if there is a specular texture it'll use next texture unit
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture_library->getSpecular(id));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        //draw a block given it's offset from the local origin
        //for example rotating a tree, the leaves would need to move / orbit with the rotation
        //rather than just each rotating and no moving anywhere
        void drawWithRotationOffset(Shader* shader, glm::vec3 offset)
        {
            //do matrix operations to generate view matrix
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            if(is_rotation)
            {
                model = glm::translate(model, -offset);
                model = glm::rotate(model, glm::radians(rotation), rotation_axis);
                model = glm::translate(model, offset);
            }
            model = glm::scale(model, scale);
            
            //each shader has "model"
            shader->setMat4("model", model);

            //first texture is diffuse
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_library->getDiffuse(id));

            //if there is a specular texture it'll use next texture unit
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture_library->getSpecular(id));

            glDrawArrays(GL_TRIANGLES, 0, 36); 
        }
};
    

#endif
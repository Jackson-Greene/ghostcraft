#ifndef TEXTURELIBRARY_H
#define TEXTURELIBRARY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader.h>
#include <stb_image.h>


//stores textures for the game
//easy to access from one global variable rather than having
//each texture a global
class TextureLibrary
{
    private:
        //have different array for diffuse and specular
        unsigned int textures_diffuse[100];
        unsigned int textures_specular[100];
        int index_diffuse;
        int index_specular;

    public:
        TextureLibrary()
        {
            index_diffuse = 0;
            index_specular = 0;
        }


        void addDiffuse(std::string texture_path, int type)
        {
            unsigned int texture;
            int width, height, nrChannels;

            stbi_set_flip_vertically_on_load(true); 

            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            
            //set texture to repeat and nearest so it doesn't get blurry when upscaled
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            
            unsigned char* texture_data;

            //depending on type (jpg or png) load the image and set/bind
            if(type == 0)
            {
                texture_data = stbi_load(FileSystem::getPath(texture_path).c_str(), &width, &height, &nrChannels, 0);
                if (texture_data)
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
                    glGenerateMipmap(GL_TEXTURE_2D);
                }
                else
                {
                    std::cout << "Failed to load texture" << std::endl;
                }
            }
            else
            {
                texture_data = stbi_load(FileSystem::getPath(texture_path).c_str(), &width, &height, &nrChannels, 0);
                if (texture_data)
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
                    glGenerateMipmap(GL_TEXTURE_2D);
                }
                else
                {
                    std::cout << "Failed to load texture" << std::endl;
                }
            }

            stbi_image_free(texture_data);
            

            textures_diffuse[index_diffuse] = texture;
            index_diffuse += 1;
        }


        void addSpecular(std::string texture_path, int type)
        {
            unsigned int texture;
            int width, height, nrChannels;

            stbi_set_flip_vertically_on_load(true); 

            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            
            //set texture to repeat and nearest so it doesn't get blurry when upscaled
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            
            unsigned char* texture_data;
            
            //depending on type (jpg or png) load the image and set/bind
            if(type == 0)
            {
                texture_data = stbi_load(FileSystem::getPath(texture_path).c_str(), &width, &height, &nrChannels, 0);
                if (texture_data)
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
                    glGenerateMipmap(GL_TEXTURE_2D);
                }
                else
                {
                    std::cout << "Failed to load texture" << std::endl;
                }
            }
            else
            {
                texture_data = stbi_load(FileSystem::getPath(texture_path).c_str(), &width, &height, &nrChannels, 0);
                if (texture_data)
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
                    glGenerateMipmap(GL_TEXTURE_2D);
                }
                else
                {
                    std::cout << "Failed to load texture" << std::endl;
                }
            }

            stbi_image_free(texture_data);
            

            textures_specular[index_specular] = texture;
            index_specular += 1;
        }


        unsigned int getDiffuse(int id)
        {
            return(textures_diffuse[id]);
        }


        unsigned int getSpecular(int id)
        {
            return(textures_specular[id]);
        }
};
    

#endif
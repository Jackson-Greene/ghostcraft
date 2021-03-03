#ifndef TERRAIN_ELEMENT_H
#define TERRAIN_ELEMENT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class terrain_element 
{       
    public:             
        glm::vec3* position;
        bool has_tree;
        char* texture_name;

    terrain_element(glm::vec3* position_, bool has_tree_, char* texture_name_)
    {
        position = position_;
        has_tree = has_tree_;
        texture_name = texture_name_;
    }
};

#endif
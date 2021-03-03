#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "block.h"
#include "tree.h"
#include "fence.h"
#include "orb.h"
#include <learnopengl/filesystem.h>
#include <stb_image.h>


//the terrain class stores information about the terrain including: blocks, trees, fences and orbs and how to draw it
//also does collision detection
class Terrain
{
    private:
        int height;
        int width;
        int depth;
        
        //terrain is still so don't really need a terrain_block_positions
        //it's local coordinates are the world coordinates
        Block** blocks;
        
        glm::vec3* tree_positions;
        int max_tree_count;
        int tree_count;

        glm::vec3* orb_positions;
        int max_orb_count;
        int orb_count;
        

    public:
        Terrain()
        {
            //set up dimensions (in blocks)
            //could have them as imports but no need for now
            height = 250;
            width = 250;
            depth = 2;
            
            max_tree_count = height * width / 10;
            tree_count = 0;

            max_orb_count = 100;
            orb_count = 0;
            
            blocks = (Block**)malloc(sizeof(Block*) * height * width * depth);
            tree_positions = (glm::vec3*)malloc(sizeof(glm::vec3) * max_tree_count);
            orb_positions = (glm::vec3*)malloc(sizeof(glm::vec3) * max_orb_count);

            generateTerrain();
        }
        

        ~Terrain()
        {
            for(int i = 0; i < height * width * depth; i++)
            {
                delete(blocks[i]);
            }
            free(blocks);

            free(tree_positions);

            free(orb_positions);
        }

        void draw(Shader* shader, Camera* camera)
        {
            glm::vec3 cam_pos = camera->getPosition();
            int cam_x = round(cam_pos.x);
            int cam_z = round(cam_pos.z);
            Block* block;
            Tree* tree = new Tree();
            Fence* fence = new Fence();

            int render_distance = 35;

            //draw blocks that are within 50 blocks of where the camera is
            for(int i = (cam_x) - render_distance; i < ((cam_x + render_distance)); i++)
            {
                for(int j = (cam_z) - render_distance; j < ((cam_z + render_distance)); j++)
                {
                    for(int k = 0; k < depth; k++)
                    {
                        if(i < height && i >= 0 && j < width && j >= 0)
                        {
                            block = getBlock(i, k, j);
                            block->draw(shader);

                            //if at edge of map then draw fences
                            if(k== 0 && (i == 0 || i == (height-1)))
                            {
                                fence->draw(shader, block->getPosition() + glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                                fence->draw(shader, block->getPosition() + glm::vec3(0.0f, 3.0f, 0.0f), 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                            }
                            
                            //if at edge of map then draw fences
                            if(k== 0 && (j == 0 || j == (width-1)))
                            {
                                fence->draw(shader, block->getPosition() + glm::vec3(0.0f, 1.0f, 0.0f));
                                fence->draw(shader, block->getPosition() + glm::vec3(0.0f, 3.0f, 0.0f));
                            }
                        }
                    }    
                }
            }

            
            //draw each tree if it's within render distance
            glm::vec3 tree_pos;
            for(int j = 0; j < tree_count - 1; j++)
            {
                tree_pos = tree_positions[j];
                
                if(abs(tree_pos.x - cam_x) < render_distance && abs(tree_pos.z - cam_z) < render_distance)
                {
                    tree->draw(shader, tree_pos);
                }
            } 

            //free memory
            delete(tree);
            delete(fence); 

        }


        void drawOrbs(Shader* shader, Camera* camera)
        {
            Orb* orb = new Orb();
            glm::vec3 cam_pos = camera->getPosition();
            int cam_x = round(cam_pos.x);
            int cam_z = round(cam_pos.z);

            int render_distance = 30;

            //draw each orb if it's within render distance
            glm::vec3 orb_pos;
            for(int k = 0; k < orb_count - 1; k++)
            {
                orb_pos = orb_positions[k];

                if(abs(orb_pos.x - cam_x) < render_distance && abs(orb_pos.z - cam_z) < render_distance)
                {
                    //movement of the orb is animated to look like it's moving around 'naturally' and rotating over time
                    //didn't use delta time because the animation didn't seem as smooth when running at high fps (assume something to do with float percision decrease as frame times becoming very low?)
                    //but speed of orb doesn't matter for gameplay so it's fine for this
                    orb->draw(shader, orb_pos + (0.5f * glm::vec3(sin(glfwGetTime()), cos(glfwGetTime()), cos(glfwGetTime()))) , glfwGetTime() * 100.0f, glm::vec3(1.0f, 1.0f, 1.0f));
                }
            }

            //free memory
            delete(orb);
        }


        void doCollisionDetection(Camera* camera)
        {
            //store previous camera position so that when the player collides with fence they can be moved back
            static glm::vec3 previous_cam_pos = glm::vec3(0.0f, 0.0f, 0.0f);  

            glm::vec3 cam_pos = camera->getPosition();
            int cam_x = round(cam_pos.x);
            int cam_z = round(cam_pos.z);

            //do terrain collision detection
            //not perfect but basically if the player is on a block just make sure the y position is updated
            //for the purpose of the assignment it works fine but could be implemented properly using more complicated algorithms
            if(cam_x >= 0 && cam_z > 0 && cam_x <= (height-1) && cam_z <= (width-1))
            {
                glm::vec3 block_pos = getBlock(cam_x, 0, cam_z)->getPosition();
                if(block_pos.y <= abs(cam_pos.y - 2.5f) || (abs(block_pos.z - cam_pos.z) <= 0.5f && abs(block_pos.x - cam_pos.x) <= 0.5f))
                {
                    camera->setYPosition(block_pos.y + 2.5f);
                }
                else
                {
                    camera->setPosition(camera->getPosition());
                }
            }

            //if collide with fence / end of map move back in previous direction
            if(cam_pos.x < 0.75f || cam_pos.x > ((float)(height-1) - 0.75f) || cam_pos.z < 0.75f || cam_pos.z > ((float)(width-1) - 0.75f))
            {
                glm::vec3 diretion_to_move = glm::normalize(cam_pos - previous_cam_pos);
                camera->setPosition(camera->getPosition() - diretion_to_move * 3.5f);
            }


            //if player is able to get out of map somehow set the position to center
            //this is more of a backup if something goes wrong at least player can keep playing
            if(cam_pos.x < -1 || cam_pos.x > height+1 || cam_pos.z < -1 || cam_pos.z > width+1)
            {
                camera->setPosition(glm::vec3(100.0f, 5.0f, 100.0f));
            }

            previous_cam_pos = cam_pos;
        }


        int doOrbCollection(Camera* camera)
        {
            int num_orbs_collected = 0;

            glm::vec3 cam_pos = camera->getPosition();

            //if player is close to orb collect it
            glm::vec3 orb_pos;
            for(int k = 0; k < orb_count - 1; k++)
            {
                orb_pos = orb_positions[k];

                if(glm::length(orb_pos - cam_pos) < 3.0f)
                {
                    num_orbs_collected += 1;

                    //orb is removed for the orb array and others shuffled down
                    for(int i = k; i < (orb_count - 1); i++)
                    {
                        orb_positions[i] = orb_positions[i+1];
                    }
                    
                    orb_count -= 1;
                }
            }

            return(num_orbs_collected);
        }



        Block* getBlock(int x, int y, int z)
        {
            //using a 1d array rather than 3d
            //this is more efficient and easier to use
            return(blocks[x + width * (y + depth * z)]);
        }

    private:

        //generating a terrain uses a height map and selects a random section of it rather than fully random terrain generation
        void generateTerrain()
        {
            int r, b, g;
            int pixel_brightness, total_brightness, average_brightness;
            int image_width, image_height, channels;

            //load height map
            unsigned char* height_map = stbi_load(FileSystem::getPath("resources/height_maps/height_map.jpg").c_str(), &image_width, &image_height, &channels, 0);
            
            int brightness_map[image_height][image_width];
            total_brightness = 0;
            int row;
            int col;

            //for every pixel in the heightmap calculate the average brightness
            for(int i = 0; i < image_height * image_width * 3; i += 3)
            {
                r = (int)(height_map[i]);
                b = (int)(height_map[i+1]);
                g = (int)(height_map[i+2]);
                pixel_brightness = ((r + b + g) / 3);

                row = ((i/3) % image_height);
                col = ((i/3) / image_width);

                //array of brightness values used to determine block height
                brightness_map[row][col] = pixel_brightness;
                total_brightness += pixel_brightness;
            }

            average_brightness = total_brightness / (image_height * image_width);

            //random selection of area in height map
            srand((unsigned int)time(NULL));
            int start_row = rand() % ((image_height - (height - 1)) - (0) + 1) + (0);
            int start_col = rand() % ((image_width - (width - 1)) - (0) + 1) + (0);

            bool tree_at_pos;
            int x, y, z;
    
            glm::vec3 position;
            glm::vec3 tree_position;
            glm::vec3 orb_position;
            
            for(int i = start_row; i < (start_row + height); i++)
            {
                for(int j = start_col; j < (start_col + width); j++)
                {  
                    for(int k = 0; k < depth; k++)
                    {
                        x = i - start_row;
                        y = k;
                        z = j - start_col;

                        position = glm::vec3((float)(x), (float)(brightness_map[i][j] - average_brightness - k), (float)(z));
                        tree_at_pos = false;


                        //depending on the brightness set the block and texture
                        if(brightness_map[i][j] - average_brightness < -3)
                        {
                            setBlock(new Block(1, position), x, y, z);
                        }
                        else if(brightness_map[i][j] - average_brightness < 0)
                        {
                            setBlock(new Block(0, position), x, y, z);
                        }
                        else if(brightness_map[i][j] - average_brightness < 1)
                        {
                            setBlock(new Block(2, position), x, y, z);

                            //trees can grow on grass have a random chance of spawning
                            if((rand() % 101) < 1 && (tree_count <= max_tree_count))
                            {
                                tree_position = glm::vec3(x, (brightness_map[i][j] - average_brightness + 1), z);
                                tree_positions[tree_count] = tree_position;
                                tree_count += 1;
                                tree_at_pos = true;
                            }
                        }
                        else if(brightness_map[i][j] - average_brightness < 3)
                        {
                            setBlock(new Block(3, position), x, y, z);
                        }
                        else
                        {
                            setBlock(new Block(4, position), x, y, z);
                        }
                        
                        //if there's not a tree then orb can go at the position
                        if(!tree_at_pos && orb_count < max_orb_count && y == 0 && ((rand() % (width * 2)) < 1))
                        {
                            orb_position = glm::vec3(x, (brightness_map[i][j] - average_brightness + 2), z);
                            orb_positions[orb_count] = orb_position;
                            orb_count += 1;
                        }                        
                    }
                }
            }
            
            stbi_image_free(height_map);
        }

        
        void setBlock(Block* block, int x, int y, int z)
        {
            //similar to getBlock using 1d array
            blocks[x + width * (y + depth * z)] = block;
        }

        
};
    

#endif
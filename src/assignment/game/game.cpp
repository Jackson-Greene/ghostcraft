//setup / structure and mouse input from https://learnopengl.com/ 
//more reference details provided in report

#include "game.h"

//set window settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 576;

//mouse settings
float last_x = SCR_WIDTH / 2.0f;
float last_y = SCR_HEIGHT / 2.0f;
bool first_mouse = true;

//timing
float delta_time = 0.0f;	
float last_frame = 0.0f;

//to make key not be pressed many times (if held for more than 1 frame)
int p_button_timer = 0;
int o_button_timer = 0;

//attenuation base settings
float attenuation_constant = 0.2f;
float attenuation_linear = 0.2f;
float attenuation_quadratic = 0.09f;

//attenuation strength as a multiple of the base
//so setting strength to 2.0f will make it 2.0x the normal value
float ambient_stength = 1.0f;

//game state
bool game_won = false;
bool game_lost = false;
bool game_started = false;
bool game_restarted = false;

//variables that are global so they can be used elsewhere
//probably don't need to be but it reduces imports and makes more sense
//in this case
TextureLibrary* texture_library;
Camera* camera;
Shader* lighting_shader;
Shader* lamp_shader;
glm::mat4 projection;


int main()
{
    //initialise in function to keep code neater
    GLFWwindow* window = initialiseOpenGl();

    texture_library = new TextureLibrary();

    //load all the textures required for the game
    loadTextures();

    //create and bind vertex vbo and vao for block
    //function call makes it neater than putting all here
    setBlockVertex();

    //set up variables from before
    camera = new Camera(glm::vec3(100.0f, 5.0f, 100.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 350.0f);
    //create shaders
    lighting_shader = new Shader("lighting_shader.vs", "lighting_shader.fs");
    lamp_shader = new Shader("lamp_shader.vs", "lamp_shader.fs");
    
    
    //setting the default values for the lighting shader
    //these don't really need to be changed as there aren't any complex material types
    //fence blocks are shiny but having a black texture for specular for the other textures
    //means they wont shine / have any specular effect
    lighting_shader->use();
	lighting_shader->setInt("material.diffuse", 0);
	lighting_shader->setInt("material.specular", 1);
    lighting_shader->setVec3("light.ambient", 0.15f, 0.15f, 0.15f);
    lighting_shader->setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
    lighting_shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    lighting_shader->setFloat("material.shininess", 100.0f);

    
    //create guis (not really guis but just texture to display)
    //number is texture id
    Gui* main_gui = new Gui(17);
    Gui* won_gui = new Gui(18);
    Gui* lost_gui = new Gui(19);
    Gui* orb_gui = new Gui(20);

    //set up game objects
    Terrain* terrain = new Terrain();
    SkyBox* skybox = new SkyBox();
    Player* player = new Player();
    Lamp* lamp = new Lamp();
    Ghost* ghost = new Ghost();
    Shrine* shrine = new Shrine();
    
    //default ghost position
    glm::vec3 ghost_position = glm::vec3(150.0f, 10.0f, 150.0f);
    bool ghost_alive = true;

    //default lamp position
    glm::vec3 lamp_position = terrain->getBlock(97, 0, 97)->getPosition() + glm::vec3(0.0f, 0.75f, 0.0f);
    bool is_holding_lamp = false;

    //default shrine position
    glm::vec3 shrine_position = terrain->getBlock(90, 0, 90)->getPosition();
    bool is_shrine_activated = false;
    float total_time_shrine_activated = 0.0f;
    
    int num_orbs_collected = 0;
    int prev_num_orbs_collected = 0;

    float total_time_orb_gui = 0.0f;
    bool display_orb_count = false;

    float total_time_game_lost = 0.0f;
    

    //render loop
    while (!glfwWindowShouldClose(window))
    {
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        //reduce timers
        if(p_button_timer > 0)
        {
            p_button_timer -= 1;
        }

        if(o_button_timer > 0)
        {
            o_button_timer -= 1;
        }
        //check inputs
        processInput(window);
        
        //background colour
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        //clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 


        if(game_restarted)
        {
            //reset win/loss/restart state
            game_started = false;
            game_won = false;
            game_lost = false;
            game_restarted = false;
            
            //make sure mouse starts at middle of screen
            last_x = SCR_WIDTH / 2.0f;
            last_y = SCR_HEIGHT / 2.0f;
            first_mouse = true;

            //reset timing
            delta_time = 0.0f;	
            last_frame = 0.0f;

            //make all lighting variables back to default (may have been changed during game)
            attenuation_constant = 0.2f;
            attenuation_linear = 0.2f;
            attenuation_quadratic = 0.09f;
            ambient_stength = 1.0f;

            //create new camera and put in at starting position and reset projection
            delete(camera);
            camera = new Camera(glm::vec3(100.0f, 5.0f, 100.0f));
            projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 350.0f);

            //regenerate terrain
            delete(terrain);
            terrain = new Terrain();

            //reset orb counters
            num_orbs_collected = 0;
            prev_num_orbs_collected = 0;
            total_time_orb_gui = 0.0f;
            display_orb_count = false;

            //reset ghost
            ghost_position = glm::vec3(150.0f, 10.0f, 150.0f);
            ghost_alive = true;

            //reset lamp
            lamp_position = terrain->getBlock(97, 0, 97)->getPosition() + glm::vec3(0.0f, 0.75f, 0.0f);
            is_holding_lamp = false;

            //reset shrine
            shrine_position = terrain->getBlock(90, 0, 90)->getPosition();
            is_shrine_activated = false;
            total_time_shrine_activated = 0.0f;  

            total_time_game_lost = 0.0f;  

            skybox->setToNight();
        }

        glm::mat4 view = camera->getViewMatrix();


        //these vectors are just normalized version without y component of camera front and camera right
        //useful to keep lamp infront of player to the right
        //a multiple of of cam_front_norm and cam_right_norm will always be the same place relative to the view / plane of the camera
        //don't want hands or lamp to follow camera pitch (up down) so no need for y component
        glm::vec3 cam_front_norm = glm::normalize(glm::vec3(camera->front.x, 0.0f, camera->front.z));
        glm::vec3 cam_right_norm = glm::normalize(glm::vec3(camera->right.x, 0.0f, camera->right.z));
    

        //use the lamp shader to draw the lamp itself and the player hands (don't want them to be effected by the light source)
        lamp_shader->use();
        lamp_shader->setMat4("view", view);
        lamp_shader->setMat4("projection", projection);

        if(!game_started)
        {
            //if the game isn't started then draw the main screen
            main_gui->draw(lamp_shader, (camera->getPosition() + cam_front_norm * 4.0f), glm::vec3(3.0f, 3.0f, 2.0f),  -(camera->yaw + 90.0f));
        }
        else if(game_won)
        {
            //if game is won then display the game won / restart screen
            //camera is reset to be sure the textured block lines up properly
            delete(camera);
            camera = new Camera(glm::vec3(100.0f, 5.0f, 100.0f));
            won_gui->draw(lamp_shader, (camera->getPosition() + cam_front_norm * 4.0f), glm::vec3(3.0f, 3.0f, 2.0f),  -(camera->yaw + 90.0f));
        }
        else if(game_lost && total_time_game_lost > 3.5f)
        {
            //if game is lost then display the game over / restart screen
            //camera is reset to be sure the textured block lines up properly
            delete(camera);
            camera = new Camera(glm::vec3(100.0f, 5.0f, 100.0f));
            lost_gui->draw(lamp_shader, (camera->getPosition() + cam_front_norm * 4.0f), glm::vec3(3.0f, 3.0f, 2.0f),  -(camera->yaw + 90.0f));
        }
        else
        {  
            //display the number of orbs collected
            //only do for 1 second
            if((prev_num_orbs_collected != num_orbs_collected || display_orb_count))
            {
                
                if(total_time_orb_gui < 1.0f)
                {  
                    if(num_orbs_collected <= 10)
                    {
                        orb_gui->setTextureId(num_orbs_collected + 20); //the orb gui textures start from number 20 so can just add 20 to get corrosponding texture 
                    }
                    else
                    {
                        orb_gui->setTextureId(31); //texture that says to go to shrine
                    }
                    
                    //display to left a bit
                    orb_gui->draw(lamp_shader, (camera->getPosition() + camera->front * 2.0f - camera->right * 1.05f + camera->up * 0.5f), glm::vec3(0.3f, 0.3f, 0.0f),  -(camera->yaw + 90.0f));
                    total_time_orb_gui += delta_time;
                    display_orb_count = true;
                }
                else
                {
                    //after 1 second then no need to display count anymore
                    display_orb_count = false;
                    total_time_orb_gui = 0.0f;
                }
            }
            
            //detect if within range of lamp
            if(glm::length(camera->getPosition() - lamp_position) < 2.5f)
            {
                //when player is close enough to the lamp then player will pick up
                is_holding_lamp = true;
            }
        
            //if player is holding then lamp go to player hand or lamp goes to shrine once it's activated (so don't enter if)
            if(is_holding_lamp && !is_shrine_activated)
            {
                //if holding follow player position
                lamp_position = (camera->getPosition() + cam_front_norm * 1.0f + cam_right_norm * 0.5f + glm::vec3(0.0f, -0.75f, 0.0f));
                //draw the lamp and animate it to rotate back and forward (sine wave)
                lamp->draw(lamp_shader, lamp_position, -(camera->yaw + 90.0f + sin(glfwGetTime()) * 350.0f * delta_time), glm::vec3(0.0f, 1.0f, 0.0f));
            }
            else
            {
                //if not holding just draw at lamp position (not being updated as player moves) and with no rotation
                lamp->draw(lamp_shader, lamp_position, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
            }

            //position of the player arms is infront of the camera slightly and down
            glm::vec3 player_model_position(camera->getPosition() + (glm::vec3(cam_front_norm.x, -1.25f, cam_front_norm.z) * 0.25f));
            //rotate hands based on yaw of camera
            float player_model_rotation = -(camera->yaw + 90.0f);
            //rotate around y axis (hands wont move up / down when mouse goes up or down)
            glm::vec3 player_model_rotation_axis(0.0f, 1.0f, 0.0f);
            //draw player
            player->draw(lamp_shader, player_model_position, player_model_rotation, player_model_rotation_axis);


            //skybox follows player(so sky doesn't look like it's moving when player moves - if it moved it would look too close to be realistic)
            glm::vec3 skybox_position = glm::vec3(camera->getPosition());
            //draw skybox
            skybox->draw(lamp_shader, skybox_position);
            

            //draw orbs, want them to be bright so use lamp shader
            if(!game_won)
            {
                terrain->drawOrbs(lamp_shader, camera);
            }
            

            //if the player is close enough to the shrine and has enough orbs activate it
            if(glm::length(camera->getPosition() - shrine_position) < 3.5f && num_orbs_collected >= 10)
            {
                is_shrine_activated = true;
            }
            

            if(is_shrine_activated)
            {
                //draw with animation and then start doing transition to win game
                shrine->drawActivated(lamp_shader, shrine_position);
                
                is_holding_lamp = false;
                lamp_position = shrine_position + glm::vec3(0.0f, 1.0f, 0.0f);
                ghost_alive = false;


                //after 5 seconds show day time skybox
                if(total_time_shrine_activated > 5.0f)
                {
                    ambient_stength = 6.0f;
                    attenuation_constant = 0.2;
                    skybox->setToDay();

                    total_time_shrine_activated += delta_time;

                    //player wins after a while so they can see the new world before game won menu shows
                    if(total_time_shrine_activated > 10.0f)
                    {
                        game_won = true;
                    }
                }
                else
                {
                    //create lighting effect like dark light going over the world slowly
                    attenuation_constant -= 0.5 * delta_time * 10.0f;
                    total_time_shrine_activated += delta_time;
                }
            }
            else
            {
                //draw shrine, want it to be easy to see so use lamp shader
                shrine->draw(lamp_shader, shrine_position);
            }
            

            //from here use the lighting shader
            lighting_shader->use();
            lighting_shader->setVec3("light.position", lamp_position);
            lighting_shader->setVec3("view_pos", camera->getPosition());
            lighting_shader->setFloat("light.constant", attenuation_constant);
            lighting_shader->setFloat("light.linear", attenuation_linear);
            lighting_shader->setFloat("light.quadratic", attenuation_quadratic);
            lighting_shader->setVec3("light.ambient", 0.15f * ambient_stength, 0.15f * ambient_stength, 0.15f * ambient_stength);
            lighting_shader->setMat4("view", view);
            lighting_shader->setMat4("projection", projection);
            

            if(ghost_alive && !game_lost)
            {
                //vector to player as unit vector (length 1)
                glm::vec3 direction_to_player = glm::normalize(camera->getPosition() - ghost_position);
                //rotation can be calulated based on components of the vector that is direction to player
                float rotation_to_player = glm::degrees(atan(direction_to_player.x / direction_to_player.z));
                //move ghost towards player (use delta_time so speed is always the same even if fps is very high or very low)
                ghost_position = ghost_position + (delta_time * 7.5f * direction_to_player);
                //draw the ghost
                ghost->draw(lighting_shader, ghost_position, rotation_to_player);

                if(glm::length(camera->getPosition() - ghost_position) < 1.5f)
                {
                    //when player is close to ghost the game is over / lost
                    game_lost = true;
                }


                //used to know if orb counter should be displayed (if same as previous nothing needs to be displayed)
                prev_num_orbs_collected = num_orbs_collected;
                //do orb collection, function returns number of orbs collected during the current frame
                num_orbs_collected += terrain->doOrbCollection(camera);
            }


            //if game is lost make it look like player is dead / laying down
            //after 3.5 seconds code will make it to a previous if statemnt that
            //displays the game lost screen
            if(game_lost)
            {
                camera->processMouseMovement(0.0f, 5.0f);
                total_time_game_lost += delta_time;
            }


            //draw terrain - includes trees, orbs and fences around map
            terrain->draw(lighting_shader, camera);

            //do collision detection
            terrain->doCollisionDetection(camera);
        }
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    //free all memory before ending
    delete(texture_library);
    delete(camera);
    delete(lighting_shader);
    delete(lamp_shader);
    delete(main_gui);
    delete(won_gui);
    delete(lost_gui);
    delete(orb_gui);
    delete(terrain);
    delete(skybox);
    delete(player);
    delete(lamp);
    delete(ghost);
    delete(shrine);

    glfwTerminate();
    return 0;
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    //store state so toggle works
    static bool is_perspective = true;
    static bool is_bright = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        game_started = true;
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        game_restarted = true;
    }

    //other controls disabled when text is being displayed
    if(game_started && !game_won && !game_lost)
    {
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            camera->processKeyboard(FORWARD, delta_time);
        }    
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            camera->processKeyboard(BACKWARD, delta_time);
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            camera->processKeyboard(LEFT, delta_time);
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            camera->processKeyboard(RIGHT, delta_time);
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            camera->processKeyboard(FORWARD, 1.2f * delta_time);
        }
        if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && o_button_timer == 0)
        {
            o_button_timer = 25;

            if(is_bright)
            {
                ambient_stength = 1.0f;
                is_bright = false;
            }
            else
            {
                ambient_stength = 5.0f;
                is_bright = true;
            }
        }
        if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && p_button_timer == 0)
        {
            p_button_timer = 25;

            if(is_perspective)
            {
                //values were found by trail and error this seems to look alright
                projection = glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -50.0f, 350.0f);
                is_perspective = false;
            }
            else
            {
                //values as before
                projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 350.0f);
                is_perspective = true;
            }
            
        } 
        if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            attenuation_linear = std::min(0.2f, (attenuation_linear + (0.20f * std::pow(attenuation_linear, 1.3f))));
            attenuation_quadratic = std::min(0.09f, (attenuation_quadratic + (0.20f * std::pow(attenuation_quadratic, 1.5f))));
        }
        if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        {
            attenuation_linear = std::max(0.035f, (attenuation_linear - (0.20f * std::pow(attenuation_linear, 1.3f))));
            attenuation_quadratic = std::max(0.00035f, (attenuation_quadratic - (0.20f * std::pow(attenuation_quadratic, 1.5f))));
        }
    }
    
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //change viewport to match window size
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //allow mouse input if game started and not in win or lost screen
    if(game_started && !game_won && !game_lost)
    {
        if (first_mouse)
        {
            last_x = xpos;
            last_y = ypos;
            first_mouse = false;
        }
        
        //get difference between previous and new
        float xoffset = xpos - last_x;
        float yoffset = last_y - ypos; // reversed since y-coordinates go from bottom to top

        last_x = xpos;
        last_y = ypos;

        //move camera view
        camera->processMouseMovement(xoffset, yoffset);
    }
}

GLFWwindow* initialiseOpenGl()
{
    //set up basic window properties
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ghost of the Orbs", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(1);
    }

    glEnable(GL_DEPTH_TEST);
    

    //sourced from https://gamedev.stackexchange.com/questions/13794/how-to-render-a-texture-partly-transparent
    //more reference details provided in report
    //enable transparency (used for ghosts and orbs)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return(window);
}


void loadTextures()
{
    //load all textures, numbers represent texture id

    texture_library->addDiffuse("resources/textures/0.png", 1);
    texture_library->addDiffuse("resources/textures/1.png", 1);
    texture_library->addDiffuse("resources/textures/2.png", 1);
    texture_library->addDiffuse("resources/textures/3.png", 1);
    texture_library->addDiffuse("resources/textures/4.png", 1);
    texture_library->addDiffuse("resources/textures/5.png", 1);
    texture_library->addDiffuse("resources/textures/6.png", 1);
    texture_library->addDiffuse("resources/textures/7.png", 1);
    texture_library->addDiffuse("resources/textures/8.png", 1);
    texture_library->addDiffuse("resources/textures/9.png", 1);
    texture_library->addDiffuse("resources/textures/10.png", 1);
    texture_library->addDiffuse("resources/textures/11.jpg", 0); //sourced from https://learnopengl.com/ //more reference details provided in report
    texture_library->addDiffuse("resources/textures/12.png", 1);
    texture_library->addDiffuse("resources/textures/13.png", 1);
    texture_library->addDiffuse("resources/textures/14.png", 1);
    texture_library->addDiffuse("resources/textures/15.png", 1);
    texture_library->addDiffuse("resources/textures/16.jpg", 0); //sourced from https://opengameart.org/content/cloudy-skyboxes //more reference details provided in report
    texture_library->addDiffuse("resources/textures/17.jpg", 0);
    texture_library->addDiffuse("resources/textures/18.jpg", 0);
    texture_library->addDiffuse("resources/textures/19.jpg", 0);
    texture_library->addDiffuse("resources/textures/20.jpg", 0);
    texture_library->addDiffuse("resources/textures/21.jpg", 0);
    texture_library->addDiffuse("resources/textures/22.jpg", 0);
    texture_library->addDiffuse("resources/textures/23.jpg", 0);
    texture_library->addDiffuse("resources/textures/24.jpg", 0);
    texture_library->addDiffuse("resources/textures/25.jpg", 0);
    texture_library->addDiffuse("resources/textures/26.jpg", 0);
    texture_library->addDiffuse("resources/textures/27.jpg", 0);
    texture_library->addDiffuse("resources/textures/28.jpg", 0);
    texture_library->addDiffuse("resources/textures/29.jpg", 0);
    texture_library->addDiffuse("resources/textures/30.jpg", 0);
    texture_library->addDiffuse("resources/textures/31.jpg", 0);

    texture_library->addSpecular("resources/textures/0s.png", 1);
    texture_library->addSpecular("resources/textures/1s.png", 1);
    texture_library->addSpecular("resources/textures/2s.png", 1);
    texture_library->addSpecular("resources/textures/3s.png", 1);
    texture_library->addSpecular("resources/textures/4s.png", 1);
    texture_library->addSpecular("resources/textures/5s.png", 1);
    texture_library->addSpecular("resources/textures/6s.png", 1);
    texture_library->addSpecular("resources/textures/7s.png", 1);
    texture_library->addSpecular("resources/textures/8s.png", 1);
    texture_library->addSpecular("resources/textures/9s.png", 1);
    texture_library->addSpecular("resources/textures/10s.png", 1);
    texture_library->addSpecular("resources/textures/11s.png", 1);
    texture_library->addSpecular("resources/textures/12s.png", 1);
    texture_library->addSpecular("resources/textures/13s.png", 1);
    texture_library->addSpecular("resources/textures/14s.png", 1);
    texture_library->addSpecular("resources/textures/15s.png", 1);
    texture_library->addSpecular("resources/textures/16s.png", 1);
    texture_library->addSpecular("resources/textures/17s.png", 1);
    texture_library->addSpecular("resources/textures/18s.png", 1);
    texture_library->addSpecular("resources/textures/19s.png", 1);
    texture_library->addSpecular("resources/textures/20s.png", 1);
    texture_library->addSpecular("resources/textures/21s.png", 1);
    texture_library->addSpecular("resources/textures/22s.png", 1);
    texture_library->addSpecular("resources/textures/23s.png", 1);
    texture_library->addSpecular("resources/textures/24s.png", 1);
    texture_library->addSpecular("resources/textures/25s.png", 1);
    texture_library->addSpecular("resources/textures/26s.png", 1);
    texture_library->addSpecular("resources/textures/27s.png", 1);
    texture_library->addSpecular("resources/textures/28s.png", 1);
    texture_library->addSpecular("resources/textures/29s.png", 1);
    texture_library->addSpecular("resources/textures/30s.png", 1);
    texture_library->addSpecular("resources/textures/31s.png", 1);
}

void setBlockVertex()
{
    //set up cube vbo and vao

    float vertices[] = 
    {
        //position            //normal             //texture coord
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}
-----------------------
About:


Ghostcraft is a game where you are being chased
by and evil ghost. The world you knew is being destroyed, 
to revert it back to it's previous state you must collect 
10 orbs and take them back to the shrine to defeat the 
ghost and live peacfully in the restored world.

-----------------------



-----------------------
How to build and run (Linux only):

- open terminal in base folder (the one this file is in)
- change directory into the "build" folder
- type the command/s 'cmake .. && make' to build the game
  (alternatively type 'cmake ..' then 'make')
- change directory into the "bin" folder
- change directory into the "assignment" folder folder
- run the command './assignment__game' to start the game

-----------------------



-----------------------
Source files (in folder "src/assignment/game"):


game.cpp, game.h 
	- the starting point and main file for the game (game.h just has includes and function definitions)

block.h 
	- contains the class that defines a Block

fence.h, ghost.h, lamp.h, orb.h, player.h, shine.h, skybox.h, tree.h 
	- contain classess for composite objects

gui.h
	- contains class that represents gui (not an actual gui but more so something to display text on the screen)

lamp_shader.fs, lamp_shader.vs, lighting_shader.fs, lighting_shader.vs
	- contains the vertex and fragment shader codes

terrain.h
	- contains Terrain class that has terrain generation and collision detection code

texture_library.h
	- contains TextureLibrary class that makes loading textures easier

-----------------------


#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;

out vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    tex_coords = vec2(a_tex_coords.x, a_tex_coords.y);
    gl_Position = projection * view * model * vec4(a_pos, 1.0);
}
//based on code from https://learnopengl.com/ and sample_2 from Antoni Liang
//more reference details provided in report

#version 330 core

out vec4 frag_colour;

struct Material 
{
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light 
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 frag_pos;  
in vec3 normal;  
in vec2 tex_coords;
  
uniform vec3 view_pos;
uniform Material material;
uniform Light light;

void main()
{
    // ambient
    vec4 ambient = vec4(light.ambient, 1.0) * texture(material.diffuse, tex_coords).rgba;
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light.position - frag_pos);
    float diff = max(dot(norm, light_direction), 0.0);
    vec4 diffuse = vec4(light.diffuse, 1.0) * diff * texture(material.diffuse, tex_coords).rgba;  
    
    // specular
    vec3 view_direction = normalize(view_pos - frag_pos);
    vec3 reflect_direction = reflect(-light_direction, norm);

    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    vec4 specular = vec4(light.specular, 1.0) * spec * vec4(texture(material.specular, tex_coords).rgb, 1.0);  


    //attenuation
    float distance = length(light.position - frag_pos);
    //equation from https://learnopengl.com/
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    //keep alpha value of original diffuse texture but multiply by attenuation
    diffuse = vec4((vec3(diffuse) * attenuation), texture(material.diffuse, tex_coords).a); //because using rgba instead of just rgb alpha value should be that of the diffuse
    specular *= vec4((vec3(specular) * attenuation), 0.0);

    vec4 result = ambient + diffuse + specular;
    
    frag_colour = result;
} 




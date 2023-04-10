#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;


out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model ; // set in code and this uniform variable are those which are not changed through out the code and we want them to be constant
uniform mat4 view ; // set in code and this uniform variable are those which are not changed through out the code and we want them to be constant
uniform mat4 projection ; // set in code and this uniform variable are those which are not changed through out the code and we want them to be constant


void main(){
   FragPos = vec3(model * vec4(aPos, 1.0)); // we want to have the lighting with respect to the world so we are taking care of only model
   Normal = mat3(transpose(inverse(model))) * aNormal; // we want the normal to point in right direction

   gl_Position = projection* view * vec4(FragPos, 1.0);
   TexCoord = aTexCoord;
}
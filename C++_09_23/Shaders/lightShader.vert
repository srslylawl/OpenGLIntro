#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 in_Normals;
out vec3 normals;
out vec3 fragPos;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main()
{
	
   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
   fragPos = vec3(modelMatrix * vec4(aPos, 1.0));
   
   normals = mat3(transpose(inverse(modelMatrix))) * in_Normals;
   //normals = in_Normals;
}
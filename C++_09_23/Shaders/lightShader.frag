#version 460
out vec4 FragColor;
in vec3 normals;
in vec3 fragPos;
uniform float time;
uniform sampler2D inputTexture;
uniform vec3 lightPosition;
uniform vec3 lightDirection;

void main()
{
	FragColor = vec4(1, 1, 1, 1);
}
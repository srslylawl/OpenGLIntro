#version 460
out vec4 FragColor;
in vec3 normals;
in vec3 fragPos;
in vec2 uv;
uniform float time;
uniform sampler2D inputTexture;
uniform sampler2D inputNormalTexture;
uniform vec3 lightPosition;
uniform vec3 lightDirection;

void main()
{
   //float modTime = mod(time, 1);
//   FragColor = texture(inputTexture, uvCoords);

	//float sinTime = sin(time);

	vec3 lDir = normalize(fragPos - lightPosition);
	
	
	//vec3 normalSample = texture(inputNormalTexture, uv).xyz; //* 2) - vec3(1,1,1);
	//normalSample *= 2;
	//normalSample -= vec3(1,1,1);
	//normalSample = normalize(normalSample);

	float lightDot = dot(-lDir, normals);
	float minLight = 0.1f;

	vec3 objectColor = vec3(texture(inputTexture, uv).xyz);

	lightDot = clamp(lightDot, minLight, 1);

	vec3 lightColor = vec3(1,.8,.8);

	vec3 finalColor = objectColor * lightColor;

	FragColor = vec4(finalColor * lightDot, 1);

	//FragColor = vec4(finalColor, .5) * lightDot;
}
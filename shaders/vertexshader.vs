#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoords;
out vec2 texCoord;
out vec3 position;
void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
   texCoord = vec2(texCoords.x, texCoords.y);
   position = aPos;
}
#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in mat4 aMatrix;
layout (location = 8) in float aInvert;

out vec2 texCoords;
out float invert;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
  texCoords = aTexCoords;
  invert = aInvert;

  gl_Position = projectionMatrix * viewMatrix * aMatrix * vec4(aPosition, 1.0);
}

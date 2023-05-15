#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 4) in mat4 aMatrix;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
  gl_Position = projectionMatrix * viewMatrix * aMatrix * vec4(aPosition, 1.0);
}

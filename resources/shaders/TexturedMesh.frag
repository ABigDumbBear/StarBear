#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D diffuse;

void main()
{
  fragColor = texture2D(diffuse, texCoords);
}

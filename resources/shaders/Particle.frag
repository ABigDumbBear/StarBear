#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D texSampler;

void main()
{
  //fragColor = texture(texSampler, texCoords);
  fragColor = vec4(1, 1, 1, 1);
}

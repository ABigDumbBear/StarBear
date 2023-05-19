#version 330 core

in vec2 texCoords;
in float invert;

out vec4 fragColor;

uniform sampler2D texSampler;

void main()
{
  //vec4 color = texture(texSampler, texCoords);
  //if(invert == 1) { color = 1.0 - color; }
  //fragColor = color;
  vec4 color = texture(texSampler, texCoords);
  color.r = invert;
  fragColor = color;
}

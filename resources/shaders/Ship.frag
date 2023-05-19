#version 330 core

in vec2 texCoords;
in float invert;

out vec4 fragColor;

uniform sampler2D texSampler;

void main()
{
  vec4 color = texture(texSampler, texCoords);
  fragColor = ((invert > 0.0) ? 1.0 - color : color);
}

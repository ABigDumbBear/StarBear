#version 330 core

in vec2 texCoords;
in float invert;

out vec4 fragColor;

uniform sampler2D texSampler;

void main()
{
  vec4 tex = texture(texSampler, texCoords);
  vec3 texColor = tex.rgb;
  float texAlpha = tex.a;

  vec3 color = ((invert > 0.0) ? 1.0 - texColor : texColor);
  fragColor = vec4(color, texAlpha);
}

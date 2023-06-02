#version 330 core

in float lifetime;
in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D texSampler;

void main()
{
  //fragColor = texture(texSampler, texCoords);
  //fragColor = vec4(1, 1, 1, 1);
  //fragColor = mix(vec4(0, 0, 1, 1), vec4(1, 1, 1, 1), lifetime);
  vec4 textureColor = texture(texSampler, texCoords);
  float textureAlpha = textureColor.a;

  fragColor = mix(vec4(0, 0, 1, textureAlpha), vec4(1, 1, 1, textureAlpha), lifetime);
}

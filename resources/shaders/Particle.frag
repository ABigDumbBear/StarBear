#version 330 core

in float lifetime;
in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D texSampler;

void main()
{
  vec4 textureColor = texture(texSampler, texCoords);
  float textureAlpha = textureColor.a;

  if(textureAlpha <= 0.9) { discard; }
  //fragColor = mix(vec4(0.458, 1, 1, textureAlpha), vec4(1, 1, 1, textureAlpha), lifetime);
  fragColor = mix(vec4(0.6, 0.6, 0.6, textureAlpha), vec4(0.5, 0.3, 0.1, textureAlpha), lifetime);
}

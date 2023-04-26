#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

namespace KumaGL {

/******************************************************************************/
Texture::Texture()
{
  glGenTextures(1, &mID);
}

/******************************************************************************/
Texture::~Texture()
{
  glDeleteTextures(1, &mID);
}

/******************************************************************************/
void Texture::LoadFromFile(const std::string& aFile, GLint aLoadFormat)
{
  int channels;
  auto data = stbi_load(aFile.c_str(),
                        &mWidth,
                        &mHeight,
                        &channels,
                        0);

  LoadFromData(data, mWidth, mHeight, aLoadFormat);

  stbi_image_free(data);
}

/******************************************************************************/
void Texture::LoadFromData(unsigned char* aData,
                           unsigned int aWidth,
                           unsigned int aHeight,
                           GLint aLoadFormat)
{
  mWidth = aWidth;
  mHeight = aHeight;

  glBindTexture(GL_TEXTURE_2D, mID);

  // Copy the image data into the currently bound texture.
  glTexImage2D(GL_TEXTURE_2D,
               0,
               aLoadFormat,
               mWidth,
               mHeight,
               0,
               aLoadFormat,
               GL_UNSIGNED_BYTE,
               aData);

  // Create a mipmap for this texture; used on small/far away objects.
  glGenerateMipmap(GL_TEXTURE_2D);
}

} // namespace KumaGL

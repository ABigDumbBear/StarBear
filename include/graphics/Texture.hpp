#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

#include <glad/glad.h>

namespace KumaGL {

class Texture
{
  public:
    Texture();
    ~Texture();
    Texture(const Texture& aTexture) = delete;
    Texture& operator=(const Texture& aTexture) = delete;
    Texture(Texture&&) = delete;
    Texture& operator=(Texture&& aMesh) = delete;

    void LoadFromFile(const std::string& aFile,
                      GLint aLoadFormat = GL_RGBA);
    void LoadFromData(unsigned char* aData,
                      unsigned int aWidth,
                      unsigned int aHeight,
                      GLint aLoadFormat = GL_RGBA);

    int GetWidth() const { return mWidth; }
    int GetHeight() const { return mHeight; }

    GLuint GetID() const { return mID; }

  private:
    int mWidth { 0 };
    int mHeight { 0 };

    GLuint mID { 0 };
};

} // namespace KumaGL

#endif

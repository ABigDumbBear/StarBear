#include "Font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>

namespace KumaGL {

FT_Library Font::mLibrary = nullptr;
int Font::mInstances = 0;

/******************************************************************************/
Font::Font()
{
  if(mLibrary == nullptr)
  {
    FT_Init_FreeType(&mLibrary);
  }

  ++mInstances;
}

/******************************************************************************/
Font::~Font()
{
  --mInstances;

  if(mInstances == 0)
  {
    FT_Done_FreeType(mLibrary);
    mLibrary = nullptr;
  }
}

/******************************************************************************/
void Font::LoadFromFile(const std::string& aFile)
{
  FT_New_Face(mLibrary, aFile.c_str(), 0, &mFace);
  CreateTexture(48);
  CreateGlyphMap(48);
}

/******************************************************************************/
void Font::UpdateMeshToDisplayText(Mesh& aMesh, const std::string& aText)
{
  // First, remove the mesh's geometry.
  aMesh.mVertices.clear();
  aMesh.mIndices.clear();

  // Next, retrieve the total height and width of the font atlas.
  float atlasWidth = mTexture.GetWidth();
  float atlasHeight = mTexture.GetHeight();

  // Next, for each character in the text, use the loaded glyph information
  // (if available) to create a vertex quad for that glyph.
  MeshVertex vertex;
  int xPos = 0;
  int yPos = 0;
  int width = 0;
  int height = 0;
  int numLines = 0;

  int indexCount = 0;
  for(const auto& character : aText)
  {
    auto glyph = mGlyphMap.find(character);
    if(glyph != mGlyphMap.end())
    {
      xPos += glyph->second.mBearingX;
      yPos = -1 * (glyph->second.mHeight - glyph->second.mBearingY);
      width = glyph->second.mWidth;
      height = glyph->second.mHeight;

      yPos -= numLines * (mFace->size->metrics.height / 64);

      // Create a quad that contains this character.
      float textureX = 0.0;
      float textureY = 0.0;

      // Bottom Left
      textureX = glyph->second.mOffsetX / atlasWidth;
      vertex.mPosition = Vec3(xPos, yPos + height, 0);
      vertex.mTexCoords[0] = textureX;
      vertex.mTexCoords[1] = textureY;
      aMesh.mVertices.emplace_back(vertex);

      // Top Left
      textureY = glyph->second.mHeight / atlasHeight;
      vertex.mPosition = Vec3(xPos, yPos, 0);
      vertex.mTexCoords[0] = textureX;
      vertex.mTexCoords[1] = textureY;
      aMesh.mVertices.emplace_back(vertex);

      // Top Right
      textureX = (glyph->second.mOffsetX + glyph->second.mWidth) / atlasWidth;
      vertex.mPosition = Vec3(xPos + width, yPos, 0);
      vertex.mTexCoords[0] = textureX;
      vertex.mTexCoords[1] = textureY;
      aMesh.mVertices.emplace_back(vertex);

      // Bottom Left
      textureX = glyph->second.mOffsetX / atlasWidth;
      textureY = 0.0;
      vertex.mPosition = Vec3(xPos, yPos + height, 0);
      vertex.mTexCoords[0] = textureX;
      vertex.mTexCoords[1] = textureY;
      aMesh.mVertices.emplace_back(vertex);

      // Top Right
      textureX = (glyph->second.mOffsetX + glyph->second.mWidth) / atlasWidth;
      textureY = glyph->second.mHeight / atlasHeight;
      vertex.mPosition = Vec3(xPos + width, yPos, 0);
      vertex.mTexCoords[0] = textureX;
      vertex.mTexCoords[1] = textureY;
      aMesh.mVertices.emplace_back(vertex);

      // Bottom Right
      textureY = 0.0;
      vertex.mPosition = Vec3(xPos + width, yPos + height, 0);
      vertex.mTexCoords[0] = textureX;
      vertex.mTexCoords[1] = textureY;
      aMesh.mVertices.emplace_back(vertex);

      // Add indices to draw this quad.
      aMesh.mIndices.emplace_back(indexCount);
      aMesh.mIndices.emplace_back(indexCount + 1);
      aMesh.mIndices.emplace_back(indexCount + 2);
      aMesh.mIndices.emplace_back(indexCount + 3);
      aMesh.mIndices.emplace_back(indexCount + 4);
      aMesh.mIndices.emplace_back(indexCount + 5);
      indexCount += 6;

      // Update the x position for the next character.
      xPos += (glyph->second.mAdvance / 64);
    }
    else if(character == '\n')
    {
      xPos = 0;
      ++numLines;
    }
  }

  // Finally, push the new geometry to the GPU.
  aMesh.UpdateVertices();
  aMesh.UpdateIndices();
}

/******************************************************************************/
void Font::CreateTexture(int aSize)
{
  // First, set the size of the font face.
  FT_Set_Pixel_Sizes(mFace, 0, aSize);

  // Next, determine the necessary height and width of the font atlas.
  // The atlas will contain the first 128 ASCII characters laid out
  // horizontally from left to right.
  unsigned int atlasHeight = 0;
  unsigned int atlasWidth = 0;
  for(unsigned int i = 32; i < 128; ++i)
  {
    auto error = FT_Load_Char(mFace, i, FT_LOAD_RENDER);
    if(error)
    {
      // If the font doesn't support this character, skip it.
      continue;
    }

    atlasWidth += mFace->glyph->bitmap.width;
    atlasHeight = std::max(atlasHeight, mFace->glyph->bitmap.rows);
  }

  // Use the height and width to create an empty texture.
  // The load format is specified as GL_RED because Freetype stores
  // image data as an 8-bit image, where each color is a single byte.
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);

  mTexture.LoadFromData(nullptr, atlasWidth, atlasHeight, GL_RED);

  // Now that we have an empty texture, fill it with the image data
  // for each of the first 128 ASCII characters.
  unsigned int xOffset = 0;
  for(unsigned int i = 32; i < 128; ++i)
  {
    auto error = FT_Load_Char(mFace, i, FT_LOAD_RENDER);
    if(error)
    {
      // If the font doesn't support this character, skip it.
      continue;
    }

    // Add the glyph data to the font atlas.
    glTexSubImage2D(GL_TEXTURE_2D,
                    0,
                    xOffset,
                    0,
                    mFace->glyph->bitmap.width,
                    mFace->glyph->bitmap.rows,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    mFace->glyph->bitmap.buffer);

    xOffset += mFace->glyph->bitmap.width;
  }

  // Generate a mipmap for the texture.
  glGenerateMipmap(GL_TEXTURE_2D);

  // After loading the texture data into memory, reset OpenGL's
  // unpack alignment to the default (4).
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glPixelStorei(GL_PACK_ALIGNMENT, 4);
}

/******************************************************************************/
void Font::CreateGlyphMap(int aSize)
{
  mGlyphMap.clear();

  // For each of the first 128 ASCII characters, access their glyph information
  // and store it in the map.
  GlyphInfo info;
  unsigned int xOffset = 0;
  for(unsigned int i = 32; i < 128; ++i)
  {
    auto error = FT_Load_Char(mFace, i, FT_LOAD_RENDER);
    if(error)
    {
      // If the curent font doesn't support this character, skip it.
      continue;
    }

    // Store the glyph's measurements in the map.
    info.mWidth = mFace->glyph->bitmap.width;
    info.mHeight = mFace->glyph->bitmap.rows;
    info.mBearingX = mFace->glyph->bitmap_left;
    info.mBearingY = mFace->glyph->bitmap_top;
    info.mAdvance = mFace->glyph->advance.x;
    info.mOffsetX = xOffset;
    mGlyphMap.emplace((char)i, info);
    
    xOffset += info.mWidth;
  }
}

} // namespace KumaGL

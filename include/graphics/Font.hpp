#ifndef FONT_HPP
#define FONT_HPP

#include <map>
#include <string>

#include "Mesh.hpp"
#include "Texture.hpp"

class FT_FaceRec_;
class FT_LibraryRec_;

namespace KumaGL {

class Font
{
  public:
    Font();
    ~Font();

    void LoadFromFile(const std::string& aFile);
    void UpdateMeshToDisplayText(Mesh& aMesh, const std::string& aText);

    const Texture& GetTexture() const { return mTexture; }

  private:
    struct GlyphInfo
    {
      unsigned int mWidth    { 0 };  // the width of the glyph
      unsigned int mHeight   { 0 };  // the height of the glyph
      unsigned int mBearingX { 0 };  // the horizontal distance from the origin
      unsigned int mBearingY { 0 };  // the vertical distance from the origin
      unsigned int mAdvance  { 0 };  // the distance between this glyph's origin and the next

      unsigned int mOffsetX  { 0 };  // the x-position of the character in the font atlas
    };

    void CreateTexture(int aSize);
    void CreateGlyphMap(int aSize);

    FT_FaceRec_* mFace = nullptr;
    Texture mTexture;
    std::map<char, GlyphInfo> mGlyphMap;

    static FT_LibraryRec_* mLibrary;
    static int mInstances;
};

} // namespace KumaGL

#endif

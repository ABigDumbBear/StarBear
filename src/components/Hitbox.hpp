#ifndef HITBOX_HPP
#define HITBOX_HPP

namespace StarBear {

struct Hitbox
{
  float x { 0 };
  float y { 0 };
  float z { 0 };

  float mWidth { 1 };
  float mHeight { 1 };
  float mDepth { 1 };

  bool mCollided { false };
};

} // namespace StarBear

#endif

#ifndef TRANSFORMNODE_HPP
#define TRANSFORMNODE_HPP

#include <memory>
#include <vector>

#include "Mat4.hpp"
#include "Vec3.hpp"
#include "MathUtil.hpp"

namespace StarBear {

struct SimpleTransform
{
  Vec3 mLocalPosition;
  Vec3 mLocalRotation;
  Vec3 mLocalScalar { 1, 1, 1 };

  Mat4 mLocalToWorld;

  Vec3 mWorldPosition;
  Vec3 mWorldRotation;
  Vec3 mWorldScalar;
};

class TransformNode
{
  public:
    TransformNode& AddChild()
    {
      mChildren.emplace_back();
      return *mChildren.back().get();
    }

    SimpleTransform mTransform;

  private:
    std::vector<std::unique_ptr<TransformNode>> mChildren;
};

} // namespace StarBear

#endif

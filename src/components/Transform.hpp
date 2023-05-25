#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <set>

#include "Mat4.hpp"
#include "Vec3.hpp"

#include "MathUtil.hpp"

#include "ECS.hpp"

namespace StarBear {

class Transform
{
  public:
    void Translate(const StarBear::Vec3& aPos)
    {
      mLocalPosition += aPos;
      UpdateMatrix();
    }

    void Rotate(float x, float y, float z)
    {
      mLocalRotation = mLocalRotation * StarBear::Rotate(Vec3(1, 0, 0), x);
      mLocalRotation = mLocalRotation * StarBear::Rotate(Vec3(0, 1, 0), y);
      mLocalRotation = mLocalRotation * StarBear::Rotate(Vec3(0, 0, 1), z);
      UpdateMatrix();
    }

    void Rotate(const StarBear::Vec3& aAxis, float aDegrees)
    {
      mLocalRotation = mLocalRotation * StarBear::Rotate(aAxis, aDegrees);
      UpdateMatrix();
    }

    void Scale(float x, float y, float z)
    {
      mLocalScalar *= Vec3(x, y, z);
      UpdateMatrix();
    }

    void SetPosition(const Vec3& aPos)
    {
      mLocalPosition = aPos;
      UpdateMatrix();
    }

    void SetRotation(float x, float y, float z)
    {
      mLocalRotation = StarBear::Rotate(Vec3(1, 0, 0), x);
      mLocalRotation = mLocalRotation * StarBear::Rotate(Vec3(0, 1, 0), y);
      mLocalRotation = mLocalRotation * StarBear::Rotate(Vec3(0, 0, 1), z);
      UpdateMatrix();
    }

    void SetRotation(const StarBear::Vec3& aAxis, float aDegrees)
    {
      mLocalRotation = StarBear::Rotate(aAxis, aDegrees);
      UpdateMatrix();
    }

    void SetScale(float x, float y, float z)
    {
      mLocalScalar = Vec3(x, y, z);
      UpdateMatrix();
    }

    const Vec3& GetPosition() const { return mLocalPosition; }
    const Mat4& GetRotation() const { return mLocalRotation; }
    const Vec3& GetScalar() const { return mLocalScalar; }
    const Mat4& GetMatrix() const { return mLocalToWorldMatrix; }

    std::set<Entity> mChildren;

  private:
    void UpdateMatrix()
    {
      mLocalToWorldMatrix = StarBear::Translate(mLocalPosition);
      mLocalToWorldMatrix = mLocalToWorldMatrix * mLocalRotation;
      mLocalToWorldMatrix = mLocalToWorldMatrix * StarBear::Scale(mLocalScalar);
    }

    Vec3 mLocalPosition;
    Mat4 mLocalRotation;
    Vec3 mLocalScalar { 1, 1, 1 };

    Mat4 mLocalToWorldMatrix;

    Vec3 mWorldPosition;
    Mat4 mWorldRotation;
    Vec3 mWorldScalar { 1, 1, 1 };
};

} // namespace StarBear

#endif

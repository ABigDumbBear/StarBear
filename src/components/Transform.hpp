#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Mat4.hpp"
#include "Vec3.hpp"

#include "MathUtil.hpp"

namespace StarBear {

class Transform
{
  public:
    void Translate(const StarBear::Vec3& aPos)
    {
      SetPosition(mLocalPosition + aPos);
    }

    void Rotate(float x, float y, float z)
    {
      SetRotation(mLocalRotation.x + x,
                  mLocalRotation.y + y,
                  mLocalRotation.z + z);
    }

    void Scale(float x, float y, float z)
    {
      SetScale(mLocalScalar.x + x,
               mLocalScalar.y + y,
               mLocalScalar.z + z);
    }

    void SetPosition(const Vec3& aPos)
    {
      mLocalPosition = aPos;
      mWorldPosition = aPos;
      UpdateMatrix();
    }

    void SetRotation(float x, float y, float z)
    {
      mLocalRotation = Vec3(x, y, z);
      mWorldRotation = Vec3(x, y, z);
      UpdateMatrix();
      UpdateAxes();
    }

    void SetScale(float x, float y, float z)
    {
      mLocalScalar = Vec3(x, y, z);
      mWorldScalar = Vec3(x, y, z);
      UpdateMatrix();
    }

    void Combine(const Transform& aParent)
    {
      UpdateMatrix();
      mMatrix = mMatrix * aParent.mMatrix;

      mWorldPosition = aParent.mMatrix * mLocalPosition;
      mWorldRotation = aParent.mWorldRotation + mLocalRotation;
      mWorldScalar.x = aParent.mWorldScalar.x * mLocalScalar.x;
      mWorldScalar.y = aParent.mWorldScalar.y * mLocalScalar.y;
      mWorldScalar.z = aParent.mWorldScalar.z * mLocalScalar.z;

      UpdateAxes();
    }

    const Vec3& GetLocalPosition() const { return mLocalPosition; }
    const Vec3& GetLocalRotation() const { return mLocalRotation; }
    const Vec3& GetLocalScalar() const { return mLocalScalar; }

    const Vec3& GetWorldPosition() const { return mWorldPosition; }
    const Vec3& GetWorldRotation() const { return mWorldRotation; }
    const Vec3& GetWorldScalar() const { return mWorldScalar; }

    const Mat4& GetMatrix() const { return mMatrix; }

    const Vec3& GetRight() const { return mRight; }
    const Vec3& GetUp() const { return mUp; }
    const Vec3& GetForward() const { return mForward; }

  private:
    void UpdateMatrix()
    {
      auto t = StarBear::Translate(mLocalPosition);
      auto r = StarBear::Rotate(Vec3(1, 0, 0), mLocalRotation.x);
      r = StarBear::Rotate(Vec3(0, 1, 0), mLocalRotation.y) * r;
      r = StarBear::Rotate(Vec3(0, 0, 1), mLocalRotation.z) * r;
      auto s = StarBear::Scale(mLocalScalar);

      mMatrix = t * r * s;
    }

    void UpdateAxes()
    {
      auto r = StarBear::Rotate(Vec3(1, 0, 0), mWorldRotation.x);
      r = StarBear::Rotate(Vec3(0, 1, 0), mWorldRotation.y) * r;
      r = StarBear::Rotate(Vec3(0, 0, 1), mWorldRotation.z) * r;

      mRight = r * Vec3(1, 0, 0);
      mUp = r * Vec3(0, 1, 0);
      mForward = r * Vec3(0, 0, -1);
    }

    // These values are relative to the object's parent (or the world,
    // if there is no parent).
    Vec3 mLocalPosition;
    Vec3 mLocalRotation;
    Vec3 mLocalScalar { 1, 1, 1 };

    // These values are relative to the world.
    Vec3 mWorldPosition;
    Vec3 mWorldRotation;
    Vec3 mWorldScalar { 1, 1, 1 };

    // Converts a vector from the object's coordinate space to
    // the world's coordinate space.
    Mat4 mMatrix;

    // These values are relative to the object itself.
    Vec3 mRight { 1, 0, 0 };
    Vec3 mUp { 0, 1, 0 };
    Vec3 mForward { 0, 0, -1 };
};

} // namespace StarBear

#endif

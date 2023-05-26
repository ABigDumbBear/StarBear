#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Mat4.hpp"
#include "Vec3.hpp"

#include "MathUtil.hpp"

namespace StarBear {

class Transform
{
  public:
    void Combine(const Transform& aParent)
    {
      mLocalToWorldMatrix = mLocalToWorldMatrix * aParent.mLocalToWorldMatrix;
      UpdateWorldCoordinates();
    }

    void Translate(const StarBear::Vec3& aPos)
    {
      mLocalPosition += aPos;
      UpdateMatrix();
      UpdateWorldCoordinates();
    }

    void Rotate(float x, float y, float z)
    {
      mLocalRotation = mLocalRotation * StarBear::Rotate(Vec3(1, 0, 0), x);
      mLocalRotation = mLocalRotation * StarBear::Rotate(Vec3(0, 1, 0), y);
      mLocalRotation = mLocalRotation * StarBear::Rotate(Vec3(0, 0, 1), z);
      UpdateMatrix();
      UpdateWorldCoordinates();
    }

    void Rotate(const StarBear::Vec3& aAxis, float aDegrees)
    {
      mLocalRotation = mLocalRotation * StarBear::Rotate(aAxis, aDegrees);
      UpdateMatrix();
      UpdateWorldCoordinates();
    }

    void Scale(float x, float y, float z)
    {
      mLocalScalar *= Vec3(x, y, z);
      UpdateMatrix();
      UpdateWorldCoordinates();
    }

    void SetPosition(const Vec3& aPos)
    {
      mLocalPosition = aPos;
      UpdateMatrix();
      UpdateWorldCoordinates();
    }

    void SetRotation(float x, float y, float z)
    {
      mLocalRotation = StarBear::Rotate(Vec3(1, 0, 0), x);
      mLocalRotation = mLocalRotation * StarBear::Rotate(Vec3(0, 1, 0), y);
      mLocalRotation = mLocalRotation * StarBear::Rotate(Vec3(0, 0, 1), z);
      UpdateMatrix();
      UpdateWorldCoordinates();
    }

    void SetRotation(const StarBear::Vec3& aAxis, float aDegrees)
    {
      mLocalRotation = StarBear::Rotate(aAxis, aDegrees);
      UpdateMatrix();
      UpdateWorldCoordinates();
    }

    void SetScale(float x, float y, float z)
    {
      mLocalScalar = Vec3(x, y, z);
      UpdateMatrix();
      UpdateWorldCoordinates();
    }

    const Vec3& GetPosition() const { return mWorldPosition; }
    const Mat4& GetRotation() const { return mWorldRotation; }
    const Vec3& GetScalar() const { return mWorldScalar; }
    const Mat4& GetMatrix() const { return mLocalToWorldMatrix; }

  private:
    void UpdateMatrix()
    {
      mLocalToWorldMatrix = StarBear::Translate(mLocalPosition);
      mLocalToWorldMatrix = mLocalToWorldMatrix * mLocalRotation;
      mLocalToWorldMatrix = mLocalToWorldMatrix * StarBear::Scale(mLocalScalar);
    }

    void UpdateWorldCoordinates()
    {
      mWorldPosition = mLocalToWorldMatrix * Vec3();
      mWorldRotation = mLocalRotation * mWorldRotation;
      mWorldScalar = mLocalToWorldMatrix * Vec3(1, 1, 1);
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

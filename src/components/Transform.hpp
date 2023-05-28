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
      mTranslationMatrix = StarBear::Translate(mPosition) * aParent.mTranslationMatrix;

      mRotationMatrix = StarBear::Rotate(Vec3(1, 0, 0), mRotation.x);
      mRotationMatrix = mRotationMatrix * StarBear::Rotate(Vec3(0, 1, 0), mRotation.y);
      mRotationMatrix = mRotationMatrix * StarBear::Rotate(Vec3(0, 0, 1), mRotation.z);
      mRotationMatrix = mRotationMatrix * aParent.mRotationMatrix;
      mForward = mRotationMatrix * Vec3(0, 0, -1);

      mScalarMatrix = StarBear::Scale(mScalar) * aParent.mScalarMatrix;
      UpdateMatrix();
    }

    void Translate(const StarBear::Vec3& aPos)
    {
      mPosition += aPos;

      mTranslationMatrix = StarBear::Translate(mPosition);
      UpdateMatrix();
    }

    void Rotate(float x, float y, float z)
    {
      mRotation += Vec3(x, y, z);

      mRotationMatrix = StarBear::Rotate(Vec3(1, 0, 0), mRotation.x);
      mRotationMatrix = mRotationMatrix * StarBear::Rotate(Vec3(0, 1, 0), mRotation.y);
      mRotationMatrix = mRotationMatrix * StarBear::Rotate(Vec3(0, 0, 1), mRotation.z);
      
      mForward = mRotationMatrix * mForward;
      UpdateMatrix();
    }

    void Scale(float x, float y, float z)
    {
      mScalar *= Vec3(x, y, z);
      mScalarMatrix = StarBear::Scale(mScalar);
      UpdateMatrix();
    }

    void SetPosition(const Vec3& aPos)
    {
      mPosition = aPos;
      mTranslationMatrix = StarBear::Translate(mPosition);
      UpdateMatrix();
    }

    void SetRotation(float x, float y, float z)
    {
      mRotation = Vec3(x, y, z);

      mRotationMatrix = StarBear::Rotate(Vec3(1, 0, 0), x);
      mRotationMatrix = mRotationMatrix * StarBear::Rotate(Vec3(0, 1, 0), y);
      mRotationMatrix = mRotationMatrix * StarBear::Rotate(Vec3(0, 0, 1), z);

      mForward = mRotationMatrix * Vec3(0, 0, -1);
      UpdateMatrix();
    }

    void SetScale(float x, float y, float z)
    {
      mScalar = Vec3(x, y, z);
      mScalarMatrix = StarBear::Scale(mScalar);
      UpdateMatrix();
    }

    const Vec3& GetPosition() const { return mPosition; }
    const Vec3& GetRotation() const { return mRotation; }
    const Vec3& GetScalar() const { return mScalar; }

    const Vec3& GetForward() const { return mForward; }

    const Mat4& GetTranslationMatrix() const { return mTranslationMatrix; }
    const Mat4& GetRotationMatrix() const { return mRotationMatrix; }
    const Mat4& GetScalarMatrix() const { return mScalarMatrix; }
    const Mat4& GetMatrix() const { return mMatrix; }

  private:
    void UpdateMatrix()
    {
      mMatrix = mTranslationMatrix * mRotationMatrix * mScalarMatrix;
    }

    Vec3 mPosition;
    Vec3 mRotation;
    Vec3 mScalar { 1, 1, 1 };

    Vec3 mForward { 0, 0, -1 };

    Mat4 mTranslationMatrix;
    Mat4 mRotationMatrix;
    Mat4 mScalarMatrix;

    Mat4 mMatrix;
};

} // namespace StarBear

#endif

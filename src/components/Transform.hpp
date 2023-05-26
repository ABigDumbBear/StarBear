#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Mat4.hpp"
#include "Vec3.hpp"

#include "MathUtil.hpp"

#include <iostream>

namespace StarBear {

class Transform
{
  public:
    void Combine(const Transform& aParent)
    {
      std::cout << "------------------------------------------------------" << std::endl;
      std::cout << "parent translation matrix: " << std::endl << aParent.mTranslationMatrix << std::endl;
      std::cout << "parent rotation matrix: " << std::endl << aParent.mRotationMatrix << std::endl;
      std::cout << "parent scalar matrix: " << std::endl << aParent.mScalarMatrix << std::endl;

      mTranslationMatrix = aParent.mTranslationMatrix * StarBear::Translate(mLocalPosition);
      mRotationMatrix = aParent.mRotationMatrix * mRotationMatrix;
      mScalarMatrix = aParent.mScalarMatrix * StarBear::Scale(mLocalScalar);

      std::cout << "child translation matrix: " << std::endl << mTranslationMatrix << std::endl;
      std::cout << "child rotation matrix: " << std::endl << mRotationMatrix << std::endl;
      std::cout << "child scalar matrix: " << std::endl << mScalarMatrix << std::endl;

      mLocalToWorldMatrix = mScalarMatrix * mRotationMatrix * mTranslationMatrix;

      mWorldPosition = mLocalToWorldMatrix * mLocalPosition;
      mWorldScalar = mLocalToWorldMatrix * mLocalScalar;
    }

    void Translate(const StarBear::Vec3& aPos)
    {
      mLocalPosition += aPos;
      Update();
    }

    void Rotate(float x, float y, float z)
    {
      mRotationMatrix = mRotationMatrix * StarBear::Rotate(Vec3(1, 0, 0), x);
      mRotationMatrix = mRotationMatrix * StarBear::Rotate(Vec3(0, 1, 0), y);
      mRotationMatrix = mRotationMatrix * StarBear::Rotate(Vec3(0, 0, 1), z);
      Update();
    }

    void Rotate(const StarBear::Vec3& aAxis, float aDegrees)
    {
      mRotationMatrix = mRotationMatrix * StarBear::Rotate(aAxis, aDegrees);
      Update();
    }

    void Scale(float x, float y, float z)
    {
      mLocalScalar *= Vec3(x, y, z);
      Update();
    }

    void SetPosition(const Vec3& aPos)
    {
      mLocalPosition = aPos;
      Update();
    }

    void SetRotation(float x, float y, float z)
    {
      mRotationMatrix = StarBear::Rotate(Vec3(1, 0, 0), x);
      mRotationMatrix = mRotationMatrix * StarBear::Rotate(Vec3(0, 1, 0), y);
      mRotationMatrix = mRotationMatrix * StarBear::Rotate(Vec3(0, 0, 1), z);
      Update();
    }

    void SetRotation(const StarBear::Vec3& aAxis, float aDegrees)
    {
      mRotationMatrix = StarBear::Rotate(aAxis, aDegrees);
      Update();
    }

    void SetScale(float x, float y, float z)
    {
      mLocalScalar = Vec3(x, y, z);
      Update();
    }

    const Vec3& GetPosition() const { return mLocalPosition; }
    const Vec3& GetScalar() const { return mLocalScalar; }

    const Vec3& GetWorldPosition() const { return mWorldPosition; }
    const Vec3& GetWorldScalar() const { return mWorldScalar; }

    const Mat4& GetMatrix() const { return mLocalToWorldMatrix; }

  private:
    void Update()
    {
      mTranslationMatrix = StarBear::Translate(mLocalPosition);
      mScalarMatrix = StarBear::Scale(mLocalScalar);

      mLocalToWorldMatrix = mTranslationMatrix * mRotationMatrix * mScalarMatrix;

      mWorldPosition = mLocalToWorldMatrix * mLocalPosition;
      mWorldScalar = mLocalToWorldMatrix * mLocalScalar;
    }

    Vec3 mLocalPosition;
    Vec3 mWorldPosition;

    Vec3 mLocalScalar { 1, 1, 1 };
    Vec3 mWorldScalar { 1, 1, 1 };

    Mat4 mTranslationMatrix;
    Mat4 mRotationMatrix;
    Mat4 mScalarMatrix;

    Mat4 mLocalToWorldMatrix;
};

} // namespace StarBear

#endif

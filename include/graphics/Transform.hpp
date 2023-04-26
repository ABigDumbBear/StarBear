#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Mat4.hpp"
#include "Vec3.hpp"

#include "MathUtil.hpp"

namespace KumaGL {

class Transform
{
  public:
    void Translate(const KumaGL::Vec3& aPos)
    {
      mPosition += aPos;
      UpdateMatrix();
    }

    void Rotate(float x, float y, float z)
    {
      mRotation = mRotation * KumaGL::Rotate(Vec3(1, 0, 0), x);
      mRotation = mRotation * KumaGL::Rotate(Vec3(0, 1, 0), y);
      mRotation = mRotation * KumaGL::Rotate(Vec3(0, 0, 1), z);
      UpdateMatrix();
    }

    void Rotate(const KumaGL::Vec3& aAxis, float aDegrees)
    {
      mRotation = mRotation * KumaGL::Rotate(aAxis, aDegrees);
      UpdateMatrix();
    }

    void Scale(float x, float y, float z)
    {
      mScalar *= Vec3(x, y, z);
      UpdateMatrix();
    }

    void SetPosition(const Vec3& aPos)
    {
      mPosition = aPos;
      UpdateMatrix();
    }

    void SetRotation(float x, float y, float z)
    {
      mRotation = KumaGL::Rotate(Vec3(1, 0, 0), x);
      mRotation = mRotation * KumaGL::Rotate(Vec3(0, 1, 0), y);
      mRotation = mRotation * KumaGL::Rotate(Vec3(0, 0, 1), z);
      UpdateMatrix();
    }

    void SetRotation(const KumaGL::Vec3& aAxis, float aDegrees)
    {
      mRotation = KumaGL::Rotate(aAxis, aDegrees);
      UpdateMatrix();
    }

    void SetScale(float x, float y, float z)
    {
      mScalar = Vec3(x, y, z);
      UpdateMatrix();
    }

    const Vec3& GetPosition() const { return mPosition; }
    const Mat4& GetRotation() const { return mRotation; }
    const Vec3& GetScalar() const { return mScalar; }

    const Mat4& GetMatrix() const { return mMatrix; }

  private:
    void UpdateMatrix()
    {
      mMatrix = KumaGL::Translate(mPosition);
      mMatrix = mMatrix * mRotation;
      mMatrix = mMatrix * KumaGL::Scale(mScalar);
    }

    Vec3 mPosition;
    Mat4 mRotation;
    Vec3 mScalar { 1, 1, 1 };

    Mat4 mMatrix;
};

} // namespace KumaGL

#endif

/*
  ==============================================================================

   This file is part of the Higgs Mythos framework.
   Copyright (c) Raw Material Software Limited

   Higgs Mythos is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Higgs Mythos is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with Higgs Mythos. If not, see <https://www.gnu.org/licenses/>.

  ==============================================================================
*/

namespace higgs
{

//==============================================================================
/**
    Holds a quaternion (a 3D vector and a scalar value).

    @tags{OpenGL}
*/
template <typename Type>
class Quaternion
{
public:
    Quaternion() noexcept                                               : scalar() {}
    Quaternion (const Quaternion& other) noexcept                       : vector (other.vector), scalar (other.scalar) {}
    Quaternion (Vector3D<Type> vectorPart, Type scalarPart) noexcept    : vector (vectorPart), scalar (scalarPart) {}
    Quaternion (Type x, Type y, Type z, Type w) noexcept                : vector (x, y, z), scalar (w) {}

    /** Creates a quaternion from an angle and an axis. */
    static Quaternion fromAngle (Type angle, Vector3D<Type> axis) noexcept
    {
        return Quaternion (axis.normalised() * std::sin (angle / (Type) 2), std::cos (angle / (Type) 2));
    }

    Quaternion& operator= (Quaternion other) noexcept
    {
        vector = other.vector;
        scalar = other.scalar;
        return *this;
    }

    Quaternion& operator*= (Quaternion other) noexcept
    {
        const Type oldScalar (scalar);
        scalar = (scalar * other.scalar) - (vector * other.vector);
        vector = (other.vector * oldScalar) + (vector * other.scalar) + (vector ^ other.vector);
        return *this;
    }

    Type length() const noexcept        { return std::sqrt (normal()); }
    Type normal() const noexcept        { return scalar * scalar + vector.lengthSquared(); }

    Quaternion normalised() const noexcept
    {
        const Type len (length());
        jassert (len > 0);
        return Quaternion (vector / len, scalar / len);
    }

    /** Returns the matrix that will perform the rotation specified by this quaternion. */
    Matrix3D<Type> getRotationMatrix() const noexcept
    {
        const Type norm (normal());
        const Type s (norm > 0 ? ((Type) 2) / norm : 0);
        const Type xs (s * vector.x),  ys (s * vector.y),  zs (s * vector.z);
        const Type wx (xs * scalar),   wy (ys * scalar),   wz (zs * scalar);
        const Type xx (xs * vector.x), xy (ys * vector.x), xz (zs * vector.x);
        const Type yy (ys * vector.y), yz (zs * vector.y), zz (zs * vector.z);

        return Matrix3D<Type> (((Type) 1) - (yy + zz), xy - wz, xz + wy, 0,
                               xy + wz, ((Type) 1) - (xx+ zz),  yz - wx, 0,
                               xz - wy, yz + wx, ((Type) 1) - (xx + yy), 0,
                               0, 0, 0, (Type) 1);
    }

    /** The vector part of the quaternion. */
    Vector3D<Type> vector;

    /** The scalar part of the quaternion. */
    Type scalar;
};

} // namespace higgs

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
    A three-coordinate vector.

    @tags{OpenGL}
*/
template <typename Type>
class Vector3D
{
public:
    Vector3D() noexcept                                        : x(), y(), z() {}
    Vector3D (Type xValue, Type yValue, Type zValue) noexcept  : x (xValue), y (yValue), z (zValue) {}
    Vector3D (const Vector3D& other) noexcept                  : x (other.x), y (other.y), z (other.z) {}
    Vector3D& operator= (Vector3D other) noexcept              { x = other.x;  y = other.y;  z = other.z; return *this; }

    /** Returns a vector that lies along the X axis. */
    static Vector3D xAxis() noexcept                        { return { (Type) 1, 0, 0 }; }
    /** Returns a vector that lies along the Y axis. */
    static Vector3D yAxis() noexcept                        { return { 0, (Type) 1, 0 }; }
    /** Returns a vector that lies along the Z axis. */
    static Vector3D zAxis() noexcept                        { return { 0, 0, (Type) 1 }; }

    Vector3D& operator+= (Vector3D other) noexcept          { x += other.x;  y += other.y;  z += other.z;  return *this; }
    Vector3D& operator-= (Vector3D other) noexcept          { x -= other.x;  y -= other.y;  z -= other.z;  return *this; }
    Vector3D& operator*= (Type scaleFactor) noexcept        { x *= scaleFactor;  y *= scaleFactor;  z *= scaleFactor;  return *this; }
    Vector3D& operator/= (Type scaleFactor) noexcept        { x /= scaleFactor;  y /= scaleFactor;  z /= scaleFactor;  return *this; }

    Vector3D operator+ (Vector3D other) const noexcept      { return { x + other.x, y + other.y, z + other.z }; }
    Vector3D operator- (Vector3D other) const noexcept      { return { x - other.x, y - other.y, z - other.z }; }
    Vector3D operator* (Type scaleFactor) const noexcept    { return { x * scaleFactor, y * scaleFactor, z * scaleFactor }; }
    Vector3D operator/ (Type scaleFactor) const noexcept    { return { x / scaleFactor, y / scaleFactor, z / scaleFactor }; }
    Vector3D operator-() const noexcept                     { return { -x, -y, -z }; }

    /** Returns the dot-product of these two vectors. */
    Type operator* (Vector3D other) const noexcept          { return x * other.x + y * other.y + z * other.z; }

    /** Returns the cross-product of these two vectors. */
    Vector3D operator^ (Vector3D other) const noexcept      { return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x }; }

    Type length() const noexcept                            { return std::sqrt (lengthSquared()); }
    Type lengthSquared() const noexcept                     { return x * x + y * y + z * z; }

    Vector3D normalised() const noexcept                    { return *this / length(); }

    /** Returns true if the vector is practically equal to the origin. */
    bool lengthIsBelowEpsilon() const noexcept
    {
        auto epsilon = std::numeric_limits<Type>::epsilon();
        return ! (x < -epsilon || x > epsilon || y < -epsilon || y > epsilon || z < -epsilon || z > epsilon);
    }

    Type x, y, z;
};

} // namespace higgs

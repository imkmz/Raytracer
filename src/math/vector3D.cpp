/*
** EPITECH PROJECT, 2025
** raytracer_b
** File description:
** vector3D
*/

#include "math/vector3D.hpp"

namespace Math {

    double Vector3D::length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    double Vector3D::dot(const Vector3D& other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3D Vector3D::normalized() const
    {
        double len = length();
        if (len == 0) return Vector3D(0, 0, 0);
        return *this / len;
    }

    Vector3D Vector3D::operator+(const Vector3D& other) const
    {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }

    Vector3D Vector3D::operator-(const Vector3D& other) const
    {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    Vector3D Vector3D::operator*(double scalar) const 
    {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    Vector3D Vector3D::operator/(double scalar) const
    {
        return Vector3D(x / scalar, y / scalar, z / scalar);
    }

    Vector3D Vector3D::operator-() const
    {
        return Vector3D(-x, -y, -z);
    }

}

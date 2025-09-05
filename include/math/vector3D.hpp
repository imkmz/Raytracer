/*
** EPITECH PROJECT, 2025
** raytracer_b
** File description:
** vector3D
*/

#pragma once
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <limits>

namespace Math {
    const double pi = 3.1415926535897932385;
    const double infinity = std::numeric_limits<double>::infinity();

    inline double degrees_to_radians(double degrees) {
        return degrees * pi / 180.0;
    }

    inline double random_double() {
        return rand() / (RAND_MAX + 1.0);
    }

    inline double random_double(double min, double max) {
        return min + (max-min)*random_double();
    }

    class Vector3D {
        public:
            double x, y, z;

            Vector3D() = default;
            Vector3D(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

            double length() const;
            double dot(const Vector3D& other) const;
            Vector3D normalized() const;

            Vector3D operator+(const Vector3D& other) const;
            Vector3D operator-(const Vector3D& other) const;
            Vector3D operator*(double scalar) const;
            Vector3D operator/(double scalar) const;
            Vector3D operator-() const;

            friend Vector3D operator*(const Vector3D& a, const Vector3D& b) {
                return Vector3D(a.x * b.x, a.y * b.y, a.z * b.z);
            }

            friend Vector3D operator*(double t, const Vector3D& v) {
                return Vector3D(t * v.x, t * v.y, t * v.z);
            }

            static Vector3D random_unit_vector() {
                auto a = random_double(0, 2*pi);
                auto z = random_double(-1, 1);
                auto r = sqrt(1 - z*z);
                return Vector3D(r*cos(a), r*sin(a), z);
            }

            Vector3D cross(const Vector3D& other) const {
                return Vector3D(
                    y * other.z - z * other.y,
                    z * other.x - x * other.z,
                    x * other.y - y * other.x
                );
            }

            static Vector3D random_in_unit_disk() {
                while (true) {
                    auto p = Vector3D(random_double(-1,1), random_double(-1,1), 0);
                    if (p.length() * p.length() >= 1) continue;
                    return p;
                }
            }

            static Vector3D random_in_unit_sphere() {
                while (true) {
                    auto p = Vector3D(random_double(-1,1), random_double(-1,1), random_double(-1,1));
                    if (p.length() * p.length() >= 1) continue;
                    return p;
                }
            }
    };
}

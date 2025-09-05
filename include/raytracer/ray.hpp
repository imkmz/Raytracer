/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** ray
*/

#pragma once
#include "math/vector3D.hpp"

class Ray {
    public:
        Ray();
        Ray(const Math::Vector3D& origin, const Math::Vector3D& direction);
    
        Math::Vector3D origin() const;
        Math::Vector3D direction() const;
        Math::Vector3D at(double t) const;
    
    private:
        Math::Vector3D orig;
        Math::Vector3D dir;
};

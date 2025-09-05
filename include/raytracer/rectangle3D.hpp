/*
** EPITECH PROJECT, 2025
** raytracer_b
** File description:
** rectangle3D
*/

#pragma once
#include "math/vector3D.hpp"

class Rectangle3D {
    public:
        Rectangle3D(const Math::Vector3D& origin,
                    const Math::Vector3D& bottomSide,
                    const Math::Vector3D& leftSide);
        
        Math::Vector3D pointAt(double u, double v) const;
        
    private:
        Math::Vector3D _origin;
        Math::Vector3D _bottomSide;
        Math::Vector3D _leftSide;
};

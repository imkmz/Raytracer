/*
** EPITECH PROJECT, 2025
** raytracer_b
** File description:
** rectangle3D
*/

#include "raytracer/rectangle3D.hpp"

Rectangle3D::Rectangle3D(const Math::Vector3D& origin,
                         const Math::Vector3D& bottomSide,
                         const Math::Vector3D& leftSide)
    : _origin(origin), _bottomSide(bottomSide), _leftSide(leftSide) {}

Math::Vector3D Rectangle3D::pointAt(double u, double v) const
{
    return _origin + _bottomSide * u + _leftSide * v;
}

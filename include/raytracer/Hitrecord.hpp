/*
** EPITECH PROJECT, 2025
** raytracer_b
** File description:
** Hitrecord
*/

#pragma once

#include "math/vector3D.hpp"

struct HitRecord {
    Math::Vector3D point;
    Math::Vector3D normal;
    double t;
    bool frontFace;

    void setFaceNormal(const Math::Vector3D& rayDirection, const Math::Vector3D& outwardNormal) {
        frontFace = rayDirection.dot(outwardNormal) < 0;
        normal = frontFace ? outwardNormal : outwardNormal * -1;
    }
};

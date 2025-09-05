/*
** EPITECH PROJECT, 2025
** raytracer_b
** File description:
** color
*/

#include "utils/color.hpp"
#include <iostream>

namespace Utils {

    void writeColor(const Math::Vector3D& color)
    {
        int r = static_cast<int>(255.999 * color.x);
        int g = static_cast<int>(255.999 * color.y);
        int b = static_cast<int>(255.999 * color.z);
        std::cout << r << " " << g << " " << b << "\n";
    }

}

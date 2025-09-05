/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Fri, May, 2025                                                     *
 * Title           - B-OOP-400-STG-4-1-raytracer-mael.kempf-le-pape                     *
 * Description     -                                                                    *
 *     translation                                                                      *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄             *
 *      ▐  ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀            *
 *        █▄▄▄▄▄  ▐  █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             *
 *        █    ▌     █          █        █        █    ▌    █         █   █             *
 *       ▄▀▄▄▄▄    ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             *
 *       █    ▐   █         █       █ █          █    ▐   █     ▐   █   █               *
 *       ▐        ▐         ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#ifndef INCLUDED_TRANSLATION_HPP
    #define INCLUDED_TRANSLATION_HPP

#include "hittable.hpp"
#include "math/vector3D.hpp"
#include <memory>

class Translation : public Hittable {
    public:
        Translation() {}
        Translation(Math::Vector3D offset, std::shared_ptr<Hittable> h)
            : offset(offset), h(h) {}

            virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    private:
        Math::Vector3D offset;
        std::shared_ptr<Hittable> h;
};

#endif

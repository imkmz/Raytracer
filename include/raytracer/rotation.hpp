/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sat, May, 2025                                                     *
 * Title           - B-OOP-400-STG-4-1-raytracer-mael.kempf-le-pape                     *
 * Description     -                                                                    *
 *     rotation                                                                         *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _|           *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _|           *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_|           *
 *       _|        _|          _|        _|      _|        _|        _|    _|           *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _|           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#ifndef INCLUDED_ROTATION_HPP
    #define INCLUDED_ROTATION_HPP

#include "hittable.hpp"
#include "math/vector3D.hpp"
#include <memory>
#include <cmath>

class Rotation : public Hittable {
    public:
        Rotation() {}
        Rotation(std::shared_ptr<Hittable> obj, const Math::Vector3D& axis, double angle)
            : object(obj), axis(axis.normalized()), angle(angle * M_PI / 180.0)
        {
            sin_ = sin(this->angle);
            cos_ = cos(this->angle);
        }

        virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    private:
        std::shared_ptr<Hittable> object;
        Math::Vector3D axis;
        double angle;
        double sin_;
        double cos_;

        Math::Vector3D rotateVector(const Math::Vector3D& v) const;
        Math::Vector3D rotateVectorInverse(const Math::Vector3D& v) const;
};

#endif
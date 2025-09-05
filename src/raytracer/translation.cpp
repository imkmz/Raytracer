/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Fri, May, 2025                                                     *
 * Title           - B-OOP-400-STG-4-1-raytracer-mael.kempf-le-pape                     *
 * Description     -                                                                    *
 *     translation                                                                      *
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

#include "raytracer/translation.hpp"

bool Translation::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const
{
    Ray translated_ray(r.origin() - offset, r.direction());

    if (!h->hit(translated_ray, t_min, t_max, rec))
        return false;
    rec.p = rec.p + offset;
    return true;
}

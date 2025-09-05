/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** hittable_list
*/

#pragma once
#include "hittable.hpp"
#include <memory>
#include <vector>

class HittableList : public Hittable {
    public:
        HittableList() {}
        HittableList(std::shared_ptr<Hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

        std::shared_ptr<Hittable> get_last() const {
            if (!objects.empty()) {
                return objects.back();
            }
            return nullptr;
        }

        void replace_last(std::shared_ptr<Hittable> new_object) {
            if (!objects.empty()) {
                objects.pop_back();
                objects.push_back(new_object);
            }
        }

        virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

    private:
        std::vector<std::shared_ptr<Hittable>> objects;
};

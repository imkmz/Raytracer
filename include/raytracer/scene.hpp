/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** scene
*/

#pragma once
#include "math/vector3D.hpp"
#include "raytracer/hittable_list.hpp"
#include "raytracer/camera.hpp"
#include <string>
#include <memory>
#include <map>
#include <vector>

class Scene {
    public:
        Scene();
        bool loadFromFile(const std::string& filename);

        int getImageWidth() const { return image_width; }
        int getImageHeight() const { return image_height; }
        int getSamplesPerPixel() const { return samples_per_pixel; }
        int getMaxDepth() const { return max_depth; }
        const Camera& getCamera() const { return camera; }
        const HittableList& getWorld() const { return world; }

    private:
        int image_width = 0;
        int image_height = 0;
        int samples_per_pixel = 0;
        int max_depth = 0;
        Camera camera;
        HittableList world;

        Math::Vector3D readVector(const std::vector<double>& values);
        Math::Vector3D readColor(const std::vector<double>& values);
};

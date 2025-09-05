/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** renderer
*/

#pragma once
#include "raytracer/scene.hpp"
#include "math/vector3D.hpp"
#include <vector>
#include <atomic>
#include <mutex>
#include <thread>
#include <iostream>

class Renderer {
    public:
        Renderer(const Scene& scene);
        void render();
        void writeImage(std::ostream& out) const;

    private:
        const Scene& scene;
        std::vector<std::vector<Math::Vector3D>> image_buffer;
        std::mutex outputMutex;
        std::atomic<int> scanlines_remaining;

        void renderScanline(int j);
};

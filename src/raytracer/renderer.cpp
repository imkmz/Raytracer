/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** renderer
*/

#include "raytracer/renderer.hpp"
#include "raytracer/utils.hpp"
#include <algorithm>

Renderer::Renderer(const Scene& scene) : scene(scene)
{
    image_buffer.resize(scene.getImageHeight(), 
                       std::vector<Math::Vector3D>(scene.getImageWidth()));
}

void Renderer::render()
{
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4;
    std::cerr << "Rendering with " << num_threads << " threads\n";

    scanlines_remaining = scene.getImageHeight();

    std::vector<std::thread> threads;
    for (int j = scene.getImageHeight()-1; j >= 0; --j) {
        while (threads.size() >= num_threads) {
            for (auto it = threads.begin(); it != threads.end(); ) {
                if (it->joinable()) {
                    it->join();
                    it = threads.erase(it);
                } else {
                    ++it;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        
        threads.emplace_back(&Renderer::renderScanline, this, j);
    }
    
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void Renderer::renderScanline(int j)
{
    const Camera& camera = scene.getCamera();
    const HittableList& world = scene.getWorld();
    const int image_width = scene.getImageWidth();
    const int image_height = scene.getImageHeight();
    const int samples_per_pixel = scene.getSamplesPerPixel();
    const int max_depth = scene.getMaxDepth();

    for (int i = 0; i < image_width; ++i) {
        Math::Vector3D pixel_color(0, 0, 0);
        for (int s = 0; s < samples_per_pixel; ++s) {
            auto u = (i + Math::random_double()) / (image_width-1);
            auto v = (j + Math::random_double()) / (image_height-1);
            Ray r = camera.get_ray(u, v);
            pixel_color = pixel_color + RaytracerUtils::ray_color(r, world, max_depth);
        }
        image_buffer[j][i] = pixel_color;
    }
    
    int remaining = --scanlines_remaining;
    std::lock_guard<std::mutex> lock(outputMutex);
    std::cerr << "\rScanlines remaining: " << remaining << ' ' << std::flush;
}

void Renderer::writeImage(std::ostream& out) const {
    out << "P3\n" << scene.getImageWidth() << " " << scene.getImageHeight() << "\n255\n";
    for (int j = scene.getImageHeight()-1; j >= 0; --j) {
        for (int i = 0; i < scene.getImageWidth(); ++i) {
            RaytracerUtils::write_color(out, image_buffer[j][i], scene.getSamplesPerPixel());
        }
    }
} 
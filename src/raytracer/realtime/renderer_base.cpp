/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Base realtime renderer implementation
*/

#include "raytracer/realtime/renderer_base.hpp"
#include "raytracer/utils.hpp"
#include <iostream>

namespace Realtime {

RendererBase::RendererBase(const Scene& scene)
    : scene(scene), running(true), scanlines_processed(0), scanlines_total(0),
      current_sample(0), progress(0), sample_done(false) 
{
    const int total_pixels = scene.getImageWidth() * scene.getImageHeight();
    pixel_colors.resize(total_pixels, Math::Vector3D(0, 0, 0));
}

RendererBase::~RendererBase()
{
    running = false;
    sample_done = true;
    queue_condition.notify_all();

    for (auto& thread : thread_pool) {
        if (thread.joinable())
            thread.join();
    }
}

void RendererBase::startRenderThreads()
{
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0)
        num_threads = 4;

    std::cout << "Starting " << num_threads << " render threads\n";

    thread_pool.resize(num_threads);
    scanlines_total = scene.getImageHeight();

    for (unsigned int i = 0; i < num_threads; ++i)
        thread_pool[i] = std::thread(&RendererBase::renderThreadFunc, this);

    current_sample.store(0);
    scanlines_processed.store(0);

    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        for (int j = 0; j < scene.getImageHeight(); ++j)
            scanline_queue.push(j);
    }

    queue_condition.notify_all();
}

void RendererBase::renderThreadFunc()
{
    const int samples_per_pixel = scene.getSamplesPerPixel();

    while (running) {
        int sample = current_sample.load();
        if (sample >= samples_per_pixel)
            break;
        int scanline = -1;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            queue_condition.wait(lock, [this] {
                return !scanline_queue.empty() || sample_done.load() || !running;
            });
            if (!running)
                break;
            if (!scanline_queue.empty()) {
                scanline = scanline_queue.front();
                scanline_queue.pop();
            }
        }
        if (scanline >= 0) {
            renderScanline(scanline, sample);
            int completed = ++scanlines_processed;
            progress.store((sample * scanlines_total + completed) / 
                         static_cast<float>(samples_per_pixel * scanlines_total) * 100.0f);
            if (completed >= scanlines_total) {
                sample_done = true;
                queue_condition.notify_all();
            }
        } else if (sample_done.load()) {
            std::lock_guard<std::mutex> lock(queue_mutex);

            current_sample.store(sample + 1);
            scanlines_processed.store(0);
            sample_done.store(false);

            for (int j = 0; j < scene.getImageHeight(); ++j)
                scanline_queue.push(j);

            queue_condition.notify_all();
        }
    }
}

}
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Base realtime renderer
*/

#ifndef REALTIME_RENDERER_BASE_HPP
#define REALTIME_RENDERER_BASE_HPP

#include "raytracer/scene.hpp"
#include "math/vector3D.hpp"
#include <vector>
#include <atomic>
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>

namespace Realtime {

class RendererBase {
public:
    RendererBase(const Scene& scene);
    virtual ~RendererBase();
    virtual void run() = 0;
protected:
    const Scene& scene;
    std::vector<Math::Vector3D> pixel_colors;
    std::mutex pixelMutex;
    std::atomic<bool> running;
    std::atomic<int> scanlines_processed;
    std::atomic<int> scanlines_total;
    std::atomic<int> current_sample;
    std::atomic<float> progress;

    std::vector<std::thread> thread_pool;
    std::queue<int> scanline_queue;
    std::mutex queue_mutex;
    std::condition_variable queue_condition;
    std::atomic<bool> sample_done;

    void startRenderThreads();
    void renderThreadFunc();
    virtual void renderScanline(int j, int sample) = 0;
    virtual void updateDisplay() = 0;
};

}

#endif
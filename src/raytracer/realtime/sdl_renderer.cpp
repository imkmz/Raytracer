/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SDL realtime renderer implementation
*/

#include "raytracer/realtime/sdl_renderer.hpp"
#include "raytracer/utils.hpp"
#include <algorithm>
#include <iostream>
#include <cmath>

namespace Realtime {

SDLRenderer::SDLRenderer(const Scene& scene)
    : RendererBase(scene), window(nullptr), renderer(nullptr), texture(nullptr)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        running = false;
        return;
    }

    window = SDL_CreateWindow("Raytracer - Rendering", 
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            scene.getImageWidth(), scene.getImageHeight(),
                            SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        running = false;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        running = false;
        return;
    }

    texture = SDL_CreateTexture(renderer, 
                             SDL_PIXELFORMAT_ARGB8888,
                             SDL_TEXTUREACCESS_STREAMING,
                             scene.getImageWidth(), scene.getImageHeight());
    if (texture == nullptr) {
        std::cerr << "Texture could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        running = false;
        return;
    }

    const int total_pixels = scene.getImageWidth() * scene.getImageHeight();
    pixels.resize(total_pixels, 0);
}

SDLRenderer::~SDLRenderer()
{
    if (texture != nullptr)
        SDL_DestroyTexture(texture);
    if (renderer != nullptr)
        SDL_DestroyRenderer(renderer);
    if (window != nullptr)
        SDL_DestroyWindow(window);
    SDL_Quit();
}

void SDLRenderer::run()
{
    if (!running)
        return;

    startRenderThreads();

    while (running) {
        handleEvents();
        updateDisplay();

        std::string title = "Raytracer - Rendering: " + 
                           std::to_string(static_cast<int>(progress.load())) + "%";
        SDL_SetWindowTitle(window, title.c_str());

        SDL_Delay(16);

        if (current_sample.load() >= scene.getSamplesPerPixel())
            running = false;
    }

    SDL_SetWindowTitle(window, "Raytracer - Render Complete");

    for (auto& thread : thread_pool) {
        if (thread.joinable())
            thread.join();
    }
}

void SDLRenderer::renderScanline(int j, int sample)
{
    const Camera& camera = scene.getCamera();
    const int image_width = scene.getImageWidth();
    const int image_height = scene.getImageHeight();
    const int max_depth = scene.getMaxDepth();

    for (int i = 0; i < image_width && running; ++i) {
        auto u = (i + Math::random_double()) / (image_width-1);
        auto v = ((image_height - 1 - j) + Math::random_double()) / (image_height-1);

        Ray r = camera.get_ray(u, v);
        Math::Vector3D color = RaytracerUtils::ray_color(r, scene.getWorld(), max_depth);

        int idx = j * image_width + i;

        std::lock_guard<std::mutex> lock(pixelMutex);
        if (sample == 0)
            pixel_colors[idx] = color;
        else {
            double weight_existing = static_cast<double>(sample) / (sample + 1);
            double weight_new = 1.0 / (sample + 1);
            pixel_colors[idx] = pixel_colors[idx] * weight_existing + color * weight_new;
        }

        pixels[idx] = colorToUint32(pixel_colors[idx]);
    }
}

void SDLRenderer::updateDisplay()
{
    std::lock_guard<std::mutex> lock(pixelMutex);

    SDL_UpdateTexture(texture, nullptr, pixels.data(), scene.getImageWidth() * sizeof(Uint32));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void SDLRenderer::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            running = false;
        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE)
                running = false;
        }
    }
}

Uint32 SDLRenderer::colorToUint32(const Math::Vector3D& color)
{
    auto r = static_cast<Uint8>(256 * std::clamp(std::sqrt(color.x), 0.0, 0.999));
    auto g = static_cast<Uint8>(256 * std::clamp(std::sqrt(color.y), 0.0, 0.999));
    auto b = static_cast<Uint8>(256 * std::clamp(std::sqrt(color.z), 0.0, 0.999));

    return (0xFF << 24) | (r << 16) | (g << 8) | b;
}

}
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** SDL realtime renderer
*/

#ifndef REALTIME_SDL_RENDERER_HPP
#define REALTIME_SDL_RENDERER_HPP

#include "raytracer/realtime/renderer_base.hpp"
#include <SDL.h>
#include <string>

namespace Realtime {

class SDLRenderer : public RendererBase {
public:
    SDLRenderer(const Scene& scene);
    ~SDLRenderer() override;

    void run() override;

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    std::vector<Uint32> pixels;

    void renderScanline(int j, int sample) override;
    void updateDisplay() override;
    void handleEvents();
    Uint32 colorToUint32(const Math::Vector3D& color);
};

}

#endif
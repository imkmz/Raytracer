/*
** EPITECH PROJECT, 2025
** raytracer_b
** File description:
** main
*/

#include "raytracer/scene.hpp"
#include "raytracer/renderer.hpp"
#include "raytracer/realtime/sdl_renderer.hpp"
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: " << argv[0] << " <config.json> [--realtime]" << std::endl;
        return 1;
    }

    Scene scene;
    if (!scene.loadFromFile(argv[1]))
        return 1;

    bool realtime = false;
    if (argc == 3 && std::string(argv[2]) == "--realtime")
        realtime = true;

    if (realtime) {
        Realtime::SDLRenderer sdlRenderer(scene);
        sdlRenderer.run();
    } else {
        Renderer renderer(scene);
        renderer.render();
        renderer.writeImage(std::cout);
    }
    std::cerr << "\nDone.\n";
    return 0;
}

#include "../include/Window.hpp"

#include <iostream>
#include <stdexcept>
#include <cstdlib>

int main() {
    vk::Window window{};

    try {
        window.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
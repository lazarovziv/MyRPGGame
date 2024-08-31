#include "Window.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

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
#include "../include/NavigateFiles.hpp"

std::string navToRoot(std::string path) {
    const std::string previousDir = "../";
    int count = 0;

    std::filesystem::path current_path = std::filesystem::current_path();

    // Traverse upwards to find the project root by checking for a file (e.g., CMakeLists.txt)
    while (current_path.has_parent_path()) {
        if (std::filesystem::exists(current_path / "CMakeLists.txt")) {
            break;  // Found the project root
        }
        current_path = current_path.parent_path();  // Move up one directory level
        count++;
    }

    for (int i = 0; i < count; ++i) {
        path.insert(0,previousDir);
    }

    return path;


}


#include "../include/PythonScriptExecutor.hpp"

bool createImage(std::string backgroundFileName, std::string foregroundFileName) {
    std::string command = "eval \"$(conda shell.bash hook)\" && conda activate tensorflow-env && python3 /Users/zivlazarov/projects/Python/LeetCode/generate_image.py ";
    std::string cmd = command + backgroundFileName + " " + foregroundFileName;

    char *finalCommand = (char *) malloc(cmd.length()+1);
    std::copy(cmd.begin(), cmd.end(), finalCommand);
    std::cout << finalCommand << std::endl;

    int result = std::system(finalCommand);

    return result == 0;
}

#include "../include/ScriptExecutor.hpp"

bool createImage(std::string backgroundFileName, std::string foregroundFileName, std::string postfixName) {
    std::string command = "eval \"$(conda shell.bash hook)\" && python3 ../generate_image.py ";
    // don't need to add "../graphics/" because we're changing directory to graphics/player in the script
    std::string cmd = command + backgroundFileName + " " + foregroundFileName + " " + postfixName;

    std::cout << cmd << std::endl;

    char *finalCommand = (char *) malloc(cmd.length()+1);
    std::copy(cmd.begin(), cmd.end(), finalCommand);
    std::cout << finalCommand << std::endl;

    int result = std::system(finalCommand);
    free(finalCommand);

    return result == 0;
}

bool concatenateImages(std::string pathToImagesDir, std::string finalName) {
    // command to concatenate all images in pathToImagesDir in a 768x2368 image to later use the createImage function to overlay it on top of the current spritesheet
    std::string cmd = "montage -mode concatenate -tile x10 -geometry +0+0 -background none ../graphics/" + pathToImagesDir + "/*.png ../graphics/player/" + finalName + ".png";

    char *finalCommand = (char *) malloc(cmd.length()+1);
    std::copy(cmd.begin(), cmd.end(), finalCommand);
    std::cout << finalCommand << std::endl;

    int result = std::system(finalCommand);
    free(finalCommand);

    return result == 0;
}

bool concatAndCreate(std::string pathToImagesDir, std::string finalName, std::string backgroundFileName, std::string foregroundFileName, std::string postfixName) {
    return concatenateImages(pathToImagesDir, finalName) && createImage(backgroundFileName, foregroundFileName, postfixName);
}

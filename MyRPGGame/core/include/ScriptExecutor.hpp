#ifndef MYRPGGAME_SCRIPTEXECUTOR_HPP
#define MYRPGGAME_SCRIPTEXECUTOR_HPP

#include <iostream>
#include <cstdlib>

//using namespace std;

bool createImage(std::string backgroundFileName, std::string foregroundFileName, std::string postfixName);
bool concatenateImages(std::string pathToImagesDir, std::string finalName);
bool concatAndCreate(std::string pathToImagesDir, std::string finalName, std::string backgroundFileName, std::string foregroundFileName, std::string postfixName);

#endif

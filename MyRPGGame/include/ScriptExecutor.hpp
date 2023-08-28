#ifndef MYRPGGAME_SCRIPTEXECUTOR_HPP
#define MYRPGGAME_SCRIPTEXECUTOR_HPP

#include <iostream>
#include <cstdlib>

using namespace std;

bool createImage(std::string backgroundFileName, std::string foregroundFileName, string postfixName);
bool concatenateImages(string pathToImagesDir, string finalName);
bool concatAndCreate(string pathToImagesDir, string finalName, string backgroundFileName, string foregroundFileName, string postfixName);

#endif

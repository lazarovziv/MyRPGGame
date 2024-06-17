#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>


void layerPngs(std::string bgPngFileName,std::string fgPngFileName,std::string postFixName);
void normalizeAlphaChannels(cv::Mat *imgToNormalize);
cv::Mat extractAlphaChannel(cv::Mat img);
int findLastSlash(std::string file);
void alphaBlend(std::string bgPngFileName, std::string fgPngFileName);
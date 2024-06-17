#include "../include/generateImg.hpp"


void layerPngs(std::string bgPngFileName, std::string fgPngFileName, std::string postFixName) {
//    bgPngFileName += ".png";
//    fgPngFileName += ".png";

    cv::Mat background = imread(bgPngFileName, cv::IMREAD_UNCHANGED);
    cv::Mat foreground = imread(fgPngFileName, cv::IMREAD_UNCHANGED);

    background.convertTo(background, CV_32FC3);
    foreground.convertTo(foreground, CV_32FC3);

    cv::Mat alphaBackground = extractAlphaChannel(background);
    cv::Mat alphaForeground = extractAlphaChannel(foreground);


    normalizeAlphaChannels(&alphaBackground);
    normalizeAlphaChannels(&alphaForeground);

//    cv::imshow("backGround",alphaBackground);
//    cv::waitKey(0);

//    cv::multiply(alphaBackground, background, background);
//    cv::multiply(alphaForeground,foreground,foreground);

    int backgroundImgLastSlash = findLastSlash(bgPngFileName);
    int foregroundImgLastSlash = findLastSlash(fgPngFileName);


}

void normalizeAlphaChannels(cv::Mat *imgToNormalize) {
    imgToNormalize->convertTo(*imgToNormalize, CV_32FC3, 1.0 / 255.0);
}

cv::Mat extractAlphaChannel(cv::Mat img) {
    std::vector<cv::Mat> channels;
    split(img, channels);
    return channels[3];
}

int findLastSlash(std::string file) {
    int idx = 0;
    for (int i = 0; i < file.length(); ++i)
        if (file[i] == '/')
            idx = i;
    return idx;
}


void alphaBlend(std::string bgPngFileName, std::string fgPngFileName) {
    cv::Mat background = imread(bgPngFileName, cv::IMREAD_UNCHANGED);
    cv::Mat foreground = imread(fgPngFileName, cv::IMREAD_UNCHANGED);

    cv::Mat bgRGB;
    cv::Mat fgRGB;
    cv::Mat bgAlpha;
    cv::Mat fgAlpha;

    std::vector<cv::Mat> bgChannels(4);
    std::vector<cv::Mat> fgChannels(4);

    cv::split(background,bgChannels);
    cv::split(foreground,fgChannels);

    cv::merge(&bgChannels[0], 3,bgRGB);
    cv::merge(&fgChannels[0],3,fgRGB);

    bgAlpha = bgChannels[3];
    fgAlpha = fgChannels[3];

    cv::Mat bgAlphaf;
    cv::Mat fgAlphaf;

    bgAlpha.convertTo(bgAlphaf,CV_32F,1.0/ 255);
    fgAlpha.convertTo(fgAlphaf,CV_32F,1.0 / 255);

    cv::Mat outAlphaf = fgAlphaf + bgAlphaf.mul(1.0 - fgAlphaf);
    cv::Mat outAlphafNonZero = outAlphaf + (outAlphaf == 0) / 255.0;

    cv::Mat outRGBf = (fgRGB.mul(fgAlphaf) + bgRGB.mul(bgAlphaf).mul(1.0 - fgAlphaf))/ outAlphafNonZero;

    cv::Mat outRGB;
    cv::Mat outAlpha;

    outRGBf.convertTo(outRGB,CV_8UC3);
    outAlphaf.convertTo(outAlpha,CV_8UC1,255);

    std::vector<cv::Mat> outChannels = {outRGB,outAlpha};
    cv::Mat outRGBA;
    cv::merge(outChannels,outRGBA);

    cv::imshow("msg",outRGBA);
    cv::waitKey(0);


}





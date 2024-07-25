#include "../include/generateImg.hpp"


void alphaBlend(std::string bgPngFileName, std::string fgPngFileName,std::string blended_image_name) {
    cv::Mat background = imread(bgPngFileName, cv::IMREAD_UNCHANGED);
    cv::Mat foreground = imread(fgPngFileName, cv::IMREAD_UNCHANGED);

    std::vector<cv::Mat> fg_channels(4), bg_channels(4);
    cv::split(foreground, fg_channels);
    cv::split(background, bg_channels);

    cv::Mat fg_alpha, bg_alpha;
    fg_channels[3].convertTo(fg_alpha, CV_32F, 1.0 / 255.0);
    bg_channels[3].convertTo(bg_alpha, CV_32F, 1.0 / 255.0);

    cv::Mat blended_alpha = fg_alpha + bg_alpha - fg_alpha.mul(bg_alpha);

    std::vector<cv::Mat> blended_channels(3);
    for (int i = 0; i < 3; ++i) {
        cv::Mat fg_rgb, bg_rgb, blended_rgb;
        fg_channels[i].convertTo(fg_rgb, CV_32F);
        bg_channels[i].convertTo(bg_rgb, CV_32F);

        cv::Mat fg_contrib = fg_rgb.mul(fg_alpha);
        cv::Mat bg_contrib = bg_rgb.mul(bg_alpha).mul(1.0 - fg_alpha);

        blended_rgb = (fg_contrib + bg_contrib).mul(1.0 / blended_alpha);

        blended_rgb.convertTo(blended_channels[i], CV_8U);
    }

    // Convert the blended alpha channel back to 8-bit
    cv::Mat blended_alpha_8u;
    blended_alpha.convertTo(blended_alpha_8u, CV_8U, 255.0);

    // Combine the blended RGB channels and the blended alpha channel
    std::vector<cv::Mat> result_channels = { blended_channels[0], blended_channels[1], blended_channels[2], blended_alpha_8u };
    cv::Mat result;
    cv::merge(result_channels, result);

    std::string final_img_name = "../../../graphics/player/spritesheets/blended_images/" + blended_image_name;
    cv::imwrite(final_img_name,result);
}





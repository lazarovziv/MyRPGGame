#ifndef MYRPGGAME_SCRIPTEXECUTOR_HPP
#define MYRPGGAME_SCRIPTEXECUTOR_HPP

#include <iostream>
#include <cstdlib>

//using namespace std;

bool create_image(std::string background_file_name, std::string foreground_file_name, std::string postfix_name);
bool concatenate_images(std::string path_to_images_dir, std::string final_name);
bool concat_and_create(std::string path_to_images_dir, std::string final_name, std::string background_file_name, std::string foreground_file_name, std::string postfix_name);

#endif

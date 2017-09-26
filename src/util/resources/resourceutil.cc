#include "resourceutil.h"
#include "logger.h"
#include <string>

const std::string ResourceUtil::IMG_PATH = "./res/img/";
const std::string ResourceUtil::RAW_PATH = "./res/raw/";
const std::string ResourceUtil::FONT_PATH = "./res/font/";

const char * ResourceUtil::image(const char * imageName) {
	return (IMG_PATH + std::string(imageName)).c_str();
}

const char * ResourceUtil::raw(const char * rawName) {
	return (RAW_PATH + std::string(rawName)).c_str();
}

const char * ResourceUtil::font(const char * fontName) {
	return (FONT_PATH + std::string(fontName)).c_str();
}
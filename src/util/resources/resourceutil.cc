#include "resourceutil.h"
#include "logger.h"
#include <string>

const std::string ResourceUtil::IMG_PATH = "./res/img/";
const std::string ResourceUtil::RAW_PATH = "./res/raw/";
const std::string ResourceUtil::FONT_PATH = "./res/font/";

std::string ResourceUtil::image(const char * imageName) {
	return (IMG_PATH + std::string(imageName));
}

std::string ResourceUtil::raw(const char * rawName) {
	return (RAW_PATH + std::string(rawName));
}

std::string ResourceUtil::font(const char * fontName) {
	return (FONT_PATH + std::string(fontName));
}
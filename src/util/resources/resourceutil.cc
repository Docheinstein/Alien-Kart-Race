#include "resourceutil.h"

#define IMG_PATH "./res/img/"
#define RAW_PATH "./res/raw/"
#define FONT_PATH "./res/font/"

std::string ResourceUtil::image(const char * imageName) {
	return (IMG_PATH + std::string(imageName));
}

std::string ResourceUtil::raw(const char * rawName) {
	return (RAW_PATH + std::string(rawName));
}

std::string ResourceUtil::font(const char * fontName) {
	return (FONT_PATH + std::string(fontName));
}
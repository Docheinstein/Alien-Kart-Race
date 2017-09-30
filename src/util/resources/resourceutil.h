#ifndef RESOURCE_UTIL_H
#define RESOURCE_UTIL_H

#include <iostream>
#include <string>

class ResourceUtil {
public:
	static const std::string IMG_PATH;
	static const std::string RAW_PATH;
	static const std::string FONT_PATH;

	static std::string image(const char * imageName);
	static std::string raw(const char * rawName);
	static std::string font(const char * fontName);
};

#endif

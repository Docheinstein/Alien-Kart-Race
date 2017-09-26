#ifndef RESOURCE_UTIL_H
#define RESOURCE_UTIL_H

#include <iostream>
#include <string>

class ResourceUtil {
public:
	static const std::string IMG_PATH;
	static const std::string RAW_PATH;
	static const std::string FONT_PATH;

	static const char * image(const char * imageName);
	static const char * raw(const char * rawName);
	static const char * font(const char * fontName);
};

#endif

#ifndef RESOURCE_UTIL_H
#define RESOURCE_UTIL_H

#include <iostream>
#include <string>

class ResourceUtil {
public:
	static const std::string IMG_PATH;
	static const std::string MAP_PATH;

	static std::string image(std::string imageName);
	static std::string map(std::string mapName);

};

#endif

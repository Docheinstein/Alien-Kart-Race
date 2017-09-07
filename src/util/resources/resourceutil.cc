#include "resourceutil.h"
#include "log.h"

const std::string ResourceUtil::IMG_PATH = "./res/img/";
const std::string ResourceUtil::MAP_PATH = "./res/raw/";

std::string ResourceUtil::image(std::string imageName) {
	return IMG_PATH + imageName;
}

std::string ResourceUtil::map(std::string mapName) {
	return MAP_PATH + mapName;
}

#ifndef RESOURCE_UTIL_H
#define RESOURCE_UTIL_H

#include <string>

/** Provides utility functions for retrieve game's resources. */
class ResourceUtil {
public:
	/**
	 * Returns the complete relative path of an image with the given filename.
	 * @param  imageName the filename of the image.
	 * @return           the complete relative path of the image.
	 */
	static std::string image(const char * imageName);

	/**
	 * Returns the complete relative path of a raw file (txt) with the given filename.
	 * @param  imageName the filename of the image.
	 * @return           the complete relative path of the raw file.
	 */
	static std::string raw(const char * rawName);

	/**
	 * Returns the complete relative path of a font with the given filename.
	 * @param  imageName the filename of the image.
	 * @return           the complete relative path of the font .
	 */
	static std::string font(const char * fontName);
};

#endif

// g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0 -c mapcreator.cc
// g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0 mapcreator.o -o mapcreator -lsfml-graphics

// ./mapcreator -inname in.png -outname out.txt

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <cstring>
#include <list>

using namespace std;

const int INVALID_PARAMETER_INDEX = -1;

const char * PARAMETER_INNAME = "-inname";
const char * PARAMETER_OUTNAME = "-outname";
const char * PARAMETER_COLOR_TO_INT = "-cti";

const char * HELP_MESSAGE =
"Mapcreator is a tool for create a txt map from a png by parsing pixels. \n \
Valid parameters: \n \
-inname <filename> // File name of the input file, e.g. -inname map.png \n \
-outname <filename> // File name of the output file, e.g. -outname map.txt \
-cti <color RGB string> <int> // Converts every pixel of the given RGB color to the given int (e.g. -cti ff56b2 12) \
";

const char * PARAMETERS_NOT_PROVIDED_MESSAGE = "Necessary parameters not provided";
const char * INVALID_PARAMETER_NUMBER_MESSAGE = "Provided wrong number of parameters";

const char * INPUT_FILE_NOT_FOUND_MESSAGE = "Input file not found";

struct ColorToInt {
    unsigned int imageColor;
    int txtInt;
} typedef CTI;

void createMap( const char *inputName, const char *outputName,
                std::list<CTI> &colorToIntList) {
    cout << "Creating map...";
    cout << "Input file: " << inputName << endl;
    cout << "Output file: " << outputName << endl;

    const string inputNameStr(inputName);

    sf::Image inputPixelMap;

    if (!inputPixelMap.loadFromFile(inputNameStr)) {
        cout << INPUT_FILE_NOT_FOUND_MESSAGE << endl;
        return;
    }

    sf::Vector2u imageSize = inputPixelMap.getSize();
    const int IMAGE_WIDTH = imageSize.x;
    const int IMAGE_HEIGHT = imageSize.y;

    const unsigned char * pixels = inputPixelMap.getPixelsPtr();
    cout << "Image size: " << IMAGE_WIDTH << "x" << IMAGE_HEIGHT << endl;
    // cout << "Pixels: " << endl;

    for (int y = 0; y < IMAGE_HEIGHT; y++) {
        for (int x = 0; x < IMAGE_WIDTH; x++) {
            unsigned int r = static_cast<unsigned int>(pixels[y * IMAGE_WIDTH * 4 + x * 4 + 0]);
            unsigned int g = static_cast<unsigned int>(pixels[y * IMAGE_WIDTH * 4 + x * 4 + 1]);
            unsigned int b = static_cast<unsigned int>(pixels[y * IMAGE_WIDTH * 4 + x * 4 + 2]);

            unsigned int hexColor = (r << 16) | (g << 8) | (b << 0);
            // int a = static_cast<unsigned int>(pixels[y * IMAGE_WIDTH * 4 + x * 4 + 3]);
            // cout << "(R:" << r << ", G:" << g << ", B:" << b << ", A:" << a << ") \t\t";
            cout << hex << hexColor << " ";
        }
        cout << endl;
    }

    for (std::list<CTI>::iterator iter = colorToIntList.begin(); iter != colorToIntList.end(); iter++) {
        cout << "Color [" << (*iter).imageColor << "] will be converted to number [" << (*iter).txtInt << "]";
    }
}

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        std::cout << HELP_MESSAGE << std::endl;
    }

    int innameIndex = INVALID_PARAMETER_INDEX;
    int outnameIndex = INVALID_PARAMETER_INDEX;
    std::list<CTI> colorToIntList;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], PARAMETER_INNAME) == 0)
            innameIndex = i + 1;
        else if (strcmp(argv[i], PARAMETER_OUTNAME) == 0)
            outnameIndex = i + 1;
        else if (strcmp(argv[i], PARAMETER_COLOR_TO_INT) == 0) {
            unsigned int imageColor;
            std::stringstream colorHexStr;
            colorHexStr << std::hex << argv[i + 1];
            colorHexStr >> imageColor;

            int txtInt = stoi(argv[i + 2]);

            colorToIntList.push_front(CTI { imageColor, txtInt });
        }
    }

    if (innameIndex > argc || outnameIndex > argc)
        cout << INVALID_PARAMETER_NUMBER_MESSAGE << endl;
    else if (innameIndex == INVALID_PARAMETER_INDEX || outnameIndex == INVALID_PARAMETER_INDEX)
        cout << PARAMETERS_NOT_PROVIDED_MESSAGE << endl;
    else
        createMap(argv[innameIndex], argv[outnameIndex], colorToIntList);
}


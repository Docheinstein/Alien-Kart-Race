// g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0 -c mapcreator.cc
// g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0 mapcreator.o -o mapcreator -lsfml-graphics

// ./mapcreator -inname in.png -outname out.txt

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <list>
#include <cstdlib>
#include <ctime>

using namespace std;

const int INVALID_PARAMETER_INDEX = -1;

const int NOT_PROVIDED_COLOR_CONVERSION_VALUE = -1;

const char * PARAMETER_INNAME = "-in";
const char * PARAMETER_OUTNAME = "-out";
const char * PARAMETER_SEPARATOR = "-separator";
const char * PARAMETER_COLOR_TO_INT = "-cti";
const char * PARAMETER_COLOR_TO_INT_RANDOM = "-cti-random";

const char * PARAMETER_SEPARATOR_DEFAULT_VALUE = " ";

const char * HELP_MESSAGE =
"Mapcreator is a tool for create a txt map from a png by parsing pixels. \n\n\
Valid parameters: \n\n\
MANDATORY \n\
-in <filename> \n\
  File name of the input file (e.g. -inname map.png) \n\n\
-out <filename> \n\
  File name of the output file \n\
  (e.g. -outname map.txt) \n\n\
OPTIONAL \n\
-cti <color RGB string> <int> \n\
  Converts every pixel of the given RGB color to the given int (e.g. -cti ff56b2 12) \n\n\
-cti-random <color RGB string> <int> <int> ... <int> \n\
  Converts every pixel of the given RGB color to one of the provided values at random (e.g. -cti-random ff56b2 12 13 14 15) \n\n\
-separator <character> \n\
  Character that will be used for separate int values on the output file (e.g. -separator ,) \n\
  If not provided, a space will be used as default value. \n\n\
General Tips:
 - Bind a color to an in int (-bind) is faster than using -no-duplicate flag, since with the duplicate\
flag every created image should be checked before find the equal one.";

const char * PARAMETERS_NOT_PROVIDED_MESSAGE = "Necessary parameters not provided";
const char * INVALID_PARAMETER_NUMBER_MESSAGE = "Provided wrong number of parameters";

const char * INPUT_FILE_NOT_FOUND_MESSAGE = "Input file not found";

const char * FILE_CREATION_ERROR_MESSAGE = "Error while creating output file";

struct ColorToInt {
    unsigned int imageColor;
    int * mapInts;
    int mapIntCount;
} typedef CTI;

void createMap( const char *inputName, const char *outputName,
                const char *separator, std::list<CTI> &colorToIntList) {
    cout << "Creating map...";
    cout << "Input file: " << inputName << endl;
    cout << "Output file: " << outputName << endl;
    cout << "Separator: " << separator << endl;
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

    for (std::list<CTI>::iterator iter = colorToIntList.begin();
        iter != colorToIntList.end();
        iter++) {
        cout << "Color [" << hex << (*iter).imageColor << "] will be converted to number(s) [ ";

        for (int i = 0; i < (*iter).mapIntCount; i++) {
            cout << dec << (*iter).mapInts[i] << " ";
        }

        cout << "]" << endl;
    }

    ofstream outFile(outputName);
    if (!outFile) {
        cout << FILE_CREATION_ERROR_MESSAGE << endl;
        return;
    }

    for (int y = 0; y < IMAGE_HEIGHT; y++) {
        for (int x = 0; x < IMAGE_WIDTH; x++) {
            unsigned int r = static_cast<unsigned int>(pixels[y * IMAGE_WIDTH * 4 + x * 4 + 0]);
            unsigned int g = static_cast<unsigned int>(pixels[y * IMAGE_WIDTH * 4 + x * 4 + 1]);
            unsigned int b = static_cast<unsigned int>(pixels[y * IMAGE_WIDTH * 4 + x * 4 + 2]);

            unsigned int hexColor = (r << 16) | (g << 8) | (b << 0);

            // cout << "(R:" << r << ", G:" << g << ", B:" << b << ", A:" << a << ") \t\t";
            // cout << hex << hexColor << " ";
            bool pixelConverted = false;

            int mapConversionInt = NOT_PROVIDED_COLOR_CONVERSION_VALUE;

            for (std::list<CTI>::iterator iter = colorToIntList.begin();
                iter != colorToIntList.end();
                iter++) {
                if (pixelConverted && hexColor == (*iter).imageColor ) {
                    cout << "Warning. <<" << hex << (*iter).imageColor << " color specified twice." << endl;
                }
                else if (hexColor == (*iter).imageColor) {
                    // Deterministic conversion
                    if ((*iter).mapIntCount == 1) {
                        mapConversionInt = (*iter).mapInts[0];
                    }
                    // Random conversion
                    else {
                        int randomIndex = rand() % (*iter).mapIntCount;
                        mapConversionInt = (*iter).mapInts[randomIndex];
                    }

                    pixelConverted = true;
                }
            }

            if (!pixelConverted) {
                cout << "Haven't found an int value for color " << hex << hexColor
                     << " will use default value: " << dec << mapConversionInt << endl;
            }

            outFile << mapConversionInt << separator;
        }
        outFile << endl;
        // cout << endl;
    }

    outFile.close();

    cout << "File created successfully" << endl;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        cout << HELP_MESSAGE << endl;
        return -1;
    }

    srand(time(NULL));

    bool invalidParameterNumber = false;

    int innameIndex = INVALID_PARAMETER_INDEX;
    int outnameIndex = INVALID_PARAMETER_INDEX;
    int separatorIndex = INVALID_PARAMETER_INDEX;

    std::list<CTI> colorToIntList;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], PARAMETER_INNAME) == 0) {
            if (++i < argc)
                innameIndex = i;
            else {
                cout << PARAMETER_INNAME << " parameter has wrong syntax" << endl;
                invalidParameterNumber = true;
            }
        }
        else if (strcmp(argv[i], PARAMETER_OUTNAME) == 0) {
            if (++i < argc)
                outnameIndex = i;
            else {
                cout << PARAMETER_OUTNAME << " parameter has wrong syntax" << endl;
                invalidParameterNumber = true;
            }
        }
        else if (strcmp(argv[i], PARAMETER_SEPARATOR) == 0) {
            if (++i < argc)
                separatorIndex = i;
            else {
                cout << PARAMETER_SEPARATOR << " parameter has wrong syntax" << endl;
                invalidParameterNumber = true;
            }
        }
        else if (strcmp(argv[i], PARAMETER_COLOR_TO_INT) == 0 ||
                strcmp(argv[i], PARAMETER_COLOR_TO_INT_RANDOM) == 0) {
            if (i + 2 < argc) {
                i++;

                unsigned int imageColor;
                std::stringstream colorHexStr;
                colorHexStr << std::hex << argv[i];
                colorHexStr >> imageColor;

                // Fill an array with the integers after this parameter
                int intCount = 0;

                i++;

                // Count the ints
                int j = i;
                while (j < argc && argv[j][0] != '-' /* End of command or next parameter found */) {
                    intCount++;
                    j++;
                }

                int *ctiInts = new int[intCount];

                // Fill with the ints
                j = 0;
                while (j < intCount) {
                    ctiInts[j] = stoi(argv[i + j]);
                    j++;
                }

                colorToIntList.push_front(CTI { imageColor, ctiInts, intCount});
            }
            else {
                cout << PARAMETER_COLOR_TO_INT << " parameter has wrong syntax" << endl;
                invalidParameterNumber = true;
            }
        }
    }

    if (invalidParameterNumber) {
        cout << INVALID_PARAMETER_NUMBER_MESSAGE << endl;
        return -1;
    }

    if (innameIndex == INVALID_PARAMETER_INDEX ||
            outnameIndex == INVALID_PARAMETER_INDEX) {
        cout << PARAMETERS_NOT_PROVIDED_MESSAGE << endl;
        return -1;
    }

    const char * separatorChar = PARAMETER_SEPARATOR_DEFAULT_VALUE;

    if (separatorIndex != INVALID_PARAMETER_INDEX) {
        separatorChar = argv[separatorIndex];
    }

    createMap(argv[innameIndex], argv[outnameIndex], separatorChar, colorToIntList);
}


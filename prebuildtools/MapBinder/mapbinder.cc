#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <list>
#include <cstdlib>
#include <ctime>

using namespace std;

/**
Logica:
Data un'immagine, la considera sezionata di una dimensione definita dall'utente (es 15x15)
e per ogni cella assegna il valore specificato dall'utente per il colore del primo pixel della
cella, o un valore di default se nulla è specificato.
*/

// Mandatory
const char * PARAMETER_INPUT_IMAGE_NAME = "-in";
const char * PARAMETER_OUT_TXT_NAME = "-out";
const char * PARAMETER_GRID_SIZE = "-size";

// Optional
const char * PARAMETER_BIND_COLOR = "-bind";

const char * MESSAGE_INPUT_FILE_NOT_FOUND = "Error: input file not found.";
const char * MESSAGE_WRONG_PARAMETER_NUMBER = "Error: every mandatory parameter should be specified.";
const char * MESSAGE_FILE_CREATION_FAILED = "Error: file creation has failed.";
const char * MESSAGE_WRONG_IMAGE_SIZE =
"Error: image has an invalid size. \
Both width and height should be a multiple of the specified grid size.";

const char * MESSAGE_HELP =
"--- Welcome to MapBinder! ---\n\n\
MinimapGenerator is a tool for: \n\
1. Create a file containing for each sub-image the value bound by the user, or a default value .\n\
Valid parameters:\n\n\
MANDATORY \n\
-in <filename> \n\
  File name of the input image file. \n\
  (e.g. -in map.png) \n\n\
-out <filename> \n\
  File name of the output txt file. \n\
  (e.g. -out map.txt) \n\n\
-size <int> \n\
  Size of a single cell of the input image. \n\
  (e.g. -size 16) \n\n\
OPTIONAL \n\
-bind <color RGB string> <int> [<int> <int> ...]\n\
  Bind a color in the image to a value in the output file. \n\
  (e.g. -bind f7a6b4 12)";

const int INVALID_PARAMETER_INDEX = -1;

const char OUT_TXT_SEPARATOR = ' ';

const int NON_BOUND_CELL_DEFAULT_VALUE = 0;

void exitWithMessage(const char *message) {
    cout << message << endl;
    exit(-1);
}

void exitWithMessage(string messageStr) {
    exitWithMessage(messageStr.c_str());
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        exitWithMessage(MESSAGE_HELP);
    }

    int inImageIndex = INVALID_PARAMETER_INDEX;
    int outTxtIndex = INVALID_PARAMETER_INDEX;
    int gridSizeIndex = INVALID_PARAMETER_INDEX;

    struct ColorToInt {
        unsigned int color;
        int intCount;
        int *ints;
    } typedef Bound;

    std::list<Bound> boundList;

    // Parse parameters
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], PARAMETER_INPUT_IMAGE_NAME) == 0) {
            if (++i < argc)
                inImageIndex = i;
            else {
                string wrongParamSyntax = PARAMETER_INPUT_IMAGE_NAME;
                wrongParamSyntax += " parameter has wrong syntax. Aborting.";
                exitWithMessage(wrongParamSyntax);
            }
        }
        else if (strcmp(argv[i], PARAMETER_OUT_TXT_NAME) == 0) {
            if (++i < argc)
                outTxtIndex = i;
            else {
                string wrongParamSyntax = PARAMETER_OUT_TXT_NAME;
                wrongParamSyntax += " parameter has wrong syntax. Aborting.";
                exitWithMessage(wrongParamSyntax);
            }
        }
        else if (strcmp(argv[i], PARAMETER_GRID_SIZE) == 0) {
            if (++i < argc)
                gridSizeIndex = i;
            else {
                string wrongParamSyntax = PARAMETER_GRID_SIZE;
                wrongParamSyntax += " parameter has wrong syntax. Aborting.";
                exitWithMessage(wrongParamSyntax);
            }
        }
        else if (strcmp(argv[i], PARAMETER_BIND_COLOR) == 0) {
            if (i < argc + 2) {
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

                int *boundInts = new int[intCount];

                // Fill with the ints
                j = 0;
                while (j < intCount) {
                    boundInts[j] = stoi(argv[i + j]);
                    j++;
                }

                boundList.push_front(Bound { imageColor, intCount, boundInts });
            }
            else {
                string wrongParamSyntax = PARAMETER_BIND_COLOR;
                wrongParamSyntax += " parameter has wrong syntax. Ignoring.";
                cout << wrongParamSyntax;
            }
        }
    }

    if (inImageIndex == INVALID_PARAMETER_INDEX ||
        outTxtIndex == INVALID_PARAMETER_INDEX ||
        gridSizeIndex == INVALID_PARAMETER_INDEX) {
        exitWithMessage(MESSAGE_WRONG_PARAMETER_NUMBER);
    }

    for (std::list<Bound>::iterator iter = boundList.begin();
        iter != boundList.end();
        iter++) {
        cout << "Color [" << hex << (*iter).color << "] will be converted to number(s) [ ";

        for (int i = 0; i < (*iter).intCount; i++) {
            cout << dec << (*iter).ints[i] << " ";
        }

        cout << "]" << endl;
    }

    // Start image parsing
    cout << "----------------------------------------------" << endl
         << "------" << "Starting image parsing" << "------" << endl
         << "----------------------------------------------" << endl;

    // Mandatory
    const char *inImageFilename = argv[inImageIndex];
    const char *outTxtFilename = argv[outTxtIndex];
    const int gridSize = stoi(argv[gridSizeIndex]);

    // Load input image
    sf::Image inputPixelMap;

    if (!inputPixelMap.loadFromFile(inImageFilename))
        exitWithMessage(MESSAGE_INPUT_FILE_NOT_FOUND);

    // Create output txt file
    ofstream outTxtFile(outTxtFilename);
    if (!outTxtFile)
        exitWithMessage(MESSAGE_FILE_CREATION_FAILED);

    sf::Vector2u imageSize = inputPixelMap.getSize();
    const int IMAGE_WIDTH = imageSize.x;
    const int IMAGE_HEIGHT = imageSize.y;

    cout << "Input image size: " << dec << IMAGE_WIDTH << "x" << IMAGE_HEIGHT << endl;

    if (IMAGE_WIDTH % gridSize != 0 || IMAGE_HEIGHT % gridSize != 0)
        exitWithMessage(MESSAGE_WRONG_IMAGE_SIZE);

    const int COLS = IMAGE_WIDTH / gridSize;
    const int ROWS = IMAGE_HEIGHT / gridSize;

    const int pixelArrayLength = gridSize * gridSize * 4;

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            int rowPixel = row * gridSize;
            int colPixel = col * gridSize;

            int boundInt = NON_BOUND_CELL_DEFAULT_VALUE;

            cout << endl << "Inspecting image at [" << row << ", " << col << "]" << endl;

            unsigned int firstCellPixel = inputPixelMap.getPixel(colPixel, rowPixel).toInteger();
            firstCellPixel = (firstCellPixel >> 8); // Ignore alpha by taking just RGB components (first 24 bits)
            firstCellPixel &= 0xffffff;

            cout << "First pixel: " << hex << firstCellPixel << dec << endl;

            // Check if it is bound
            for (std::list<Bound>::iterator iter = boundList.begin();
                iter != boundList.end();
                iter++) {
                if (firstCellPixel == (*iter).color ) {
                    // Deterministic conversion
                    if ((*iter).intCount == 1) {
                        boundInt = (*iter).ints[0];
                        cout << "Found a parameter bound for cell at [" << row << ", " << col
                             << "]. Using int {" << boundInt << "}" << endl;

                    }
                    // Random conversion
                    else {
                        int randomIndex = rand() % (*iter).intCount;
                        boundInt = (*iter).ints[randomIndex];
                            cout << "Found a parameter bound for cell at [" << row << ", " << col
                                 << "]. Using random int {" << boundInt << "}" << endl;
                    }
                }
            }

            outTxtFile << boundInt << OUT_TXT_SEPARATOR;
        }
        outTxtFile << endl;
    }

    outTxtFile.close();

    cout << "Process completed." << endl;
}
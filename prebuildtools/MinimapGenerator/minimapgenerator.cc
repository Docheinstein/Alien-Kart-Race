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
e per ogni cella determina in base ai pixel contenuti, se è da considerarsi piena o vuota
per una eventuale rappresentazione semplificata per una minimappa.
*/

// Mandatory
const char * PARAMETER_INPUT_IMAGE_NAME = "-in";
const char * PARAMETER_OUT_TXT_NAME = "-out";
const char * PARAMETER_GRID_SIZE = "-size";

// Optional
const char * PARAMETER_FULL_COLOR = "-full";
const char * PARAMETER_EMPTY_COLOR = "-empty";

const char * MESSAGE_INPUT_FILE_NOT_FOUND = "Error: input file not found.";
const char * MESSAGE_WRONG_PARAMETER_NUMBER = "Error: every mandatory parameter should be specified.";
const char * MESSAGE_FILE_CREATION_FAILED = "Error: file creation has failed.";
const char * MESSAGE_WRONG_IMAGE_SIZE =
"Error: image has an invalid size. \
Both width and height should be a multiple of the specified grid size.";

const char * MESSAGE_HELP =
"--- Welcome to MinimapGenerator! ---\n\n\
MinimapGenerator is a tool for: \n\
1. Determine for each sub-image of the specified size for the given image, if the cell is \
full or empty based on the given association (using -full or -empty).\n\
Valid parameters:\n\n\
MANDATORY \n\
-in <filename> \n\
  File name of the input image file. \n\
  (e.g. -inname map.png) \n\n\
-out <filename> \n\
  File name of the output txt file. \n\
  (e.g. -outname map.txt) \n\n\
-size <int> \n\
  Size of a single cell of the input image. \n\
  (e.g. -size 16) \n\n\
OPTIONAL \n\
-full <color RGB string> [<color RGB string> <color RGB string>  ...]\n\
  Considers the pixels of the given color(s) as a full. \n\
  (e.g. -full f7a6b4) \n\
  (e.g. -full f7a6b4 f8n7a2 a4b5c5) \n\n\
-empty <color RGB string> [<color RGB string> <color RGB string>  ...]\n\
  Considers the pixels of the given color(s) as a empty. \n\
  (e.g. -empty f7a6b4) \n\
  (e.g. -empty f7a6b4 f8n7a2 a4b5c5)";

const int INVALID_PARAMETER_INDEX = -1;

const char OUT_TXT_SEPARATOR = ' ';

const char FULL_CELL_CHAR = '1';
const char EMPTY_CELL_CHAR = '0';

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

    std::list<unsigned int> fullColorList;
    std::list<unsigned int> emptyColorList;

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
        else if (strcmp(argv[i], PARAMETER_FULL_COLOR) == 0) {
            if (++i < argc) {
                while (i < argc && argv[i][0] != '-' /* End of command or next parameter found */) {

                    unsigned int fullColor;
                    std::stringstream colorHexStr;
                    colorHexStr << std::hex << argv[i];
                    colorHexStr >> fullColor;

                    fullColorList.push_front(fullColor);

                    i++;
                }
                i--;
            }
            else {
                string wrongParamSyntax = PARAMETER_FULL_COLOR;
                wrongParamSyntax += " parameter has wrong syntax. Ignoring.";
                cout << wrongParamSyntax;
            }
        }
        else if (strcmp(argv[i], PARAMETER_EMPTY_COLOR) == 0) {
            if (++i < argc) {
                while (i < argc && argv[i][0] != '-' /* End of command or next parameter found */) {

                    unsigned int emptyColor;
                    std::stringstream colorHexStr;
                    colorHexStr << std::hex << argv[i];
                    colorHexStr >> emptyColor;

                    emptyColorList.push_front(emptyColor);

                    i++;
                }
                i--;
            }
            else {
                string wrongParamSyntax = PARAMETER_EMPTY_COLOR;
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

    for (std::list<unsigned int>::iterator iter = emptyColorList.begin();
        iter != emptyColorList.end();
        iter++) {
        cout << "Color [" << hex << (*iter) << "] will be considered as empty" << endl;
    }

    for (std::list<unsigned int>::iterator iter = fullColorList.begin();
        iter != fullColorList.end();
        iter++) {
        cout << "Color [" << hex << (*iter) << "] will be considered as full" << endl;
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

    sf::Image currentCellImage;

    const int pixelArrayLength = gridSize * gridSize * 4;

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            int rowPixel = row * gridSize;
            int colPixel = col * gridSize;

            currentCellImage.create(gridSize, gridSize);
            currentCellImage.copy(inputPixelMap, 0, 0, sf::IntRect(colPixel, rowPixel, gridSize, gridSize));

            const unsigned char * currentCellPixels = currentCellImage.getPixelsPtr();

            cout << endl << "Inspecting image at [" << dec << row << ", " << col << "]" << endl;

            int emptyPixelCount = 0;
            int fullPixelCount = 0;

            for (int i = 0; i < pixelArrayLength; i++) {

                unsigned int r = static_cast<unsigned int>(currentCellPixels[i++]);
                unsigned int g = static_cast<unsigned int>(currentCellPixels[i++]);
                unsigned int b = static_cast<unsigned int>(currentCellPixels[i++]);
                // Ignore alpha component

                unsigned int hexColor = (r << 16) | (g << 8) | (b /* << 0 */);

                for (std::list<unsigned int>::iterator iter = emptyColorList.begin();
                    iter != emptyColorList.end();
                    iter++) {

                    if ((*iter) == hexColor) {
                        emptyPixelCount++;
                        break;
                    }
                }
                for (std::list<unsigned int>::iterator iter = fullColorList.begin();
                    iter != fullColorList.end();
                    iter++) {

                    if ((*iter) == hexColor) {
                        fullPixelCount++;
                        break;
                    }
                }
            }

            if (emptyPixelCount > fullPixelCount) {
                outTxtFile << EMPTY_CELL_CHAR << OUT_TXT_SEPARATOR;
                cout << "  is empty (Empty:" << emptyPixelCount << " ; Full: " << fullPixelCount << ")" << endl;
            }
            else {
                outTxtFile << FULL_CELL_CHAR << OUT_TXT_SEPARATOR;
                cout << "  is full (Empty:" << emptyPixelCount << " ; Full: " << fullPixelCount << ")" << endl;
            }
        }
        outTxtFile << endl;
    }

    outTxtFile.close();

    cout << "Process completed." << endl;
}
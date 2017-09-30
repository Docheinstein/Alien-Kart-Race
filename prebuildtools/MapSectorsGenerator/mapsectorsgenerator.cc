#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <list>
#include <cstdlib>
#include <limits>

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
"--- Welcome to MapSectorsGenerator! ---\n\n\
MapSectorsGenerator is a tool for: \n\
1. Found quads in the image by considering each sub-image of the given size, and taking them as\
    quad's corner if match the bound color. The output file will contain, the coordinate of\
    the top-left, top-right, down-right, down-left and the bound value for each found quad.\n\
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
  Bind a color in the image that represents the corners of the quad, to a value in the output file. \n\
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
        int boundInt;
    } typedef Bound;

    struct Coordinate {
        int row;
        int col;
    } typedef Coordinate;

    struct BoundQuad {
        Coordinate ul, ur, dr, dl;
        unsigned int color;
        int boundInt;
    }   typedef BoundQuad;

    std::list<Bound> boundList;
    std::list<BoundQuad> quadList;

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

                i++;

                boundList.push_back(Bound { imageColor, stoi(argv[i]) });
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
        cout << "Quad with corners of color [" << hex << (*iter).color << "] will be converted to number(s) [ ";
        cout << dec << (*iter).boundInt << " ";


        BoundQuad q;
        q.ul = Coordinate {numeric_limits<int>::max(), numeric_limits<int>::max()};
        q.ur = Coordinate {numeric_limits<int>::max(), -1};
        q.dr = Coordinate {-1, -1};
        q.dl = Coordinate {-1, numeric_limits<int>::max()};

        q.color = (*iter).color;
        q.boundInt = (*iter).boundInt;
        quadList.push_back(q);

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

            //  cout << endl << "Inspecting image at [" << row << ", " << col << "]" << endl;

            unsigned int firstCellPixel = inputPixelMap.getPixel(colPixel, rowPixel).toInteger();
            firstCellPixel = (firstCellPixel >> 8); // Ignore alpha by taking just RGB components (first 24 bits)
            firstCellPixel &= 0xffffff;

            //  cout << "First pixel: " << hex << firstCellPixel << dec << endl;

            // Check if it is bound
            for (std::list<BoundQuad>::iterator qIter = quadList.begin();
                qIter != quadList.end();
                qIter++) {
                if (firstCellPixel == (*qIter).color) {
                    if (row <= (*qIter).ul.row && col <= (*qIter).ul.col)
                        (*qIter).ul = Coordinate { row, col };
                    if (row <= (*qIter).ur.row && col >= (*qIter).ur.col)
                        (*qIter).ur = Coordinate { row, col };
                    if (row >= (*qIter).dr.row && col >= (*qIter).dr.col)
                        (*qIter).dr = Coordinate { row, col };
                    if (row >= (*qIter).dl.row && col <= (*qIter).dl.col)
                        (*qIter).dl = Coordinate { row, col };
                }
            }
        }
    }

    for (std::list<BoundQuad>::iterator qIter = quadList.begin();
        qIter != quadList.end();
        qIter++) {
        cout << "Found quad with color [" << hex << (*qIter).color << dec << "] with coords:" << endl
             << "UL: " << (*qIter).ul.row << ", " << (*qIter).ul.col << endl
             << "UR: " << (*qIter).ur.row << ", " << (*qIter).ur.col << endl
             << "DR: " << (*qIter).dr.row << ", " << (*qIter).dr.col << endl
             << "DL: " << (*qIter).dl.row << ", " << (*qIter).dl.col << endl
             << "Type bound:" << (*qIter).boundInt << endl
             << "--------------" << endl;

         outTxtFile
            << (*qIter).ul.row << OUT_TXT_SEPARATOR << (*qIter).ul.col << OUT_TXT_SEPARATOR
            << (*qIter).ur.row << OUT_TXT_SEPARATOR << (*qIter).ur.col << OUT_TXT_SEPARATOR
            << (*qIter).dr.row << OUT_TXT_SEPARATOR << (*qIter).dr.col << OUT_TXT_SEPARATOR
            << (*qIter).dl.row << OUT_TXT_SEPARATOR << (*qIter).dl.col << OUT_TXT_SEPARATOR
            << (*qIter).boundInt << endl;
    }

    outTxtFile.close();

    cout << "Process completed." << endl;
}
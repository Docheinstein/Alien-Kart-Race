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
e per ogni cella crea un'immagine di quella dimensione, eventualmente evitando di creare di
uguali, con una certa numerazione.
*/

// Mandatory
const char * PARAMETER_INPUT_IMAGE_NAME = "-in";
const char * PARAMETER_OUT_TXT_NAME = "-out";
const char * PARAMETER_GRID_SIZE = "-size";

// Optional
const char * PARAMETER_BIND = "-bind";
const char * PARAMETER_NO_DUPLICATE = "-no-duplicate";

const char * PARAMETER_OUT_IMAGES_DIR_NAME = "-out-images-dir";
const char * PARAMETER_OUT_IMAGES_PREFIX = "-out-images-prefix";
const char * PARAMETER_OUT_IMAGES_START_FROM = "-out-images-from";
const char * PARAMETER_TILESET = "-tileset"

const char * MESSAGE_INPUT_FILE_NOT_FOUND = "Error: input file not found.";
const char * MESSAGE_WRONG_PARAMETER_NUMBER = "Error: every mandatory parameter should be specified.";
const char * MESSAGE_FILE_CREATION_FAILED = "Error: file creation has failed.";
const char * MESSAGE_WRONG_IMAGE_SIZE =
"Error: image has an invalid size. \
Both width and height should be a multiple of the specified grid size.";

const char * MESSAGE_HELP =
"--- Welcome to MapStripper! ---\n\n\
MapStripper is a tool for: \n\
1. Create sub-images by stripping the original image for the specified grid size.\n\
2. Create a txt map using the bound integers or the integers associated with the newly created images.\n\n\
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
-bind <color RGB string> <int> [<int> <int> ...]\n\
  Checks the first pixel of each cell of the specified grid size, if the color matches\
 the specified one, doesn't create an image for that cell and instead associated it with the given int. \n\
  More than an int can be passed as parameter, in that case, the cell will be assocaited with\
 an int taken at random between the specified ones. \n\
  (e.g. -bind ff3ab5 12) \n\
  (e.g. -bind ff3ab5 12 13 15 17) \n\n\
-no-duplicate \n\
  If a parsed cell image is equals to a previosuly created image, doesn't create the image and instead\
  use the int associated with the old cell for this cell too. (Can slow down the process, but the result is optimized). \n\
  (e.g. -no-duplicate) \n\n\
-out-images-dir <path_without_final_slash>\n\
  The directory the images will be saved into. \n\
  (e.g. -out-images-dir img) \n\n\
-out-images-prefix <string>\n\
  The prefix that will be used for the created images. \n\
  (e.g. -out-images-prefix mymap_) \n\n\
-out-images-from <int> \n\
  The enumeration of the created images will start from the specified int instead of from 0. \n\
  (e.g. -out-images-from 21)";

/*
\n\n\
-tileset <width> <height>\n\
Creates an unique tileset of the specified size instead of single images.
The size of the tileset should be a multiple of the grid size.
The tileset is filled starting from the first row, in horizontal;\
when the first row is filled, the second will be used, and so on...\n\
If this
(e.g. tileset 512 512)"

*/
const int INVALID_PARAMETER_INDEX = -1;

const char OUT_TXT_SEPARATOR = ' ';

void exitWithMessage(const char *message) {
    cout << message << endl;
    exit(-1);
}

void exitWithMessage(string messageStr) {
    exitWithMessage(messageStr.c_str());
}

struct ColorBindInt {
    unsigned int imageColor;
    int * mapInts;
    int mapIntCount;
} typedef Bound;

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        exitWithMessage(MESSAGE_HELP);
    }

    srand(time(NULL));

    int inImageIndex = INVALID_PARAMETER_INDEX;
    int outTxtIndex = INVALID_PARAMETER_INDEX;
    int gridSizeIndex = INVALID_PARAMETER_INDEX;

    int outImagesDirIndex = INVALID_PARAMETER_INDEX;
    int outImagesPrefixIndex = INVALID_PARAMETER_INDEX;

    int startFromIndex = INVALID_PARAMETER_INDEX;

    bool noDuplicate = false;
    bool createTileset = false;

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
        else if (strcmp(argv[i], PARAMETER_OUT_IMAGES_DIR_NAME) == 0) {
            if (++i < argc)
                outImagesDirIndex = i;
            else {
                string wrongParamSyntax = PARAMETER_OUT_IMAGES_DIR_NAME;
                wrongParamSyntax += " parameter has wrong syntax. Ignoring.";
                cout << wrongParamSyntax;
            }
        }
        else if (strcmp(argv[i], PARAMETER_BIND) == 0) {
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

                int *boundInts = new int[intCount];

                // Fill with the ints
                j = 0;
                while (j < intCount) {
                    boundInts[j] = stoi(argv[i + j]);
                    j++;
                }

                boundList.push_front(Bound { imageColor, boundInts, intCount});
            }
            else {
                string wrongParamSyntax = PARAMETER_BIND;
                wrongParamSyntax += " parameter has wrong syntax. Ignoring.";
                cout << wrongParamSyntax;
            }
        }
        else if (strcmp(argv[i], PARAMETER_NO_DUPLICATE) == 0) {
            noDuplicate = true;
        }
        else if (strcmp(argv[i], PARAMETER_TILESET) == 0) {
            createTileset = true;
        }
        else if (strcmp(argv[i], PARAMETER_OUT_IMAGES_START_FROM) == 0) {
            if (++i < argc)
                startFromIndex = i;
            else {
                string wrongParamSyntax = PARAMETER_OUT_IMAGES_START_FROM;
                wrongParamSyntax += " parameter has wrong syntax. Ignoring.";
                cout << wrongParamSyntax;
            }
        }
        else if (strcmp(argv[i], PARAMETER_OUT_IMAGES_PREFIX) == 0) {
            if (++i < argc)
                outImagesPrefixIndex = i;
            else {
                string wrongParamSyntax = PARAMETER_OUT_IMAGES_PREFIX;
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
        cout << "Color [" << hex << (*iter).imageColor << "] will be converted to number(s) [ ";

        for (int i = 0; i < (*iter).mapIntCount; i++) {
            cout << dec << (*iter).mapInts[i] << " ";
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

    // Optional
    const char *outImagesDir = "./";
    const char *outImagesPrefix = "";

    if (outImagesDirIndex != INVALID_PARAMETER_INDEX)
        outImagesDir = argv[outImagesDirIndex];

    int startFrom = 0;
    if (startFromIndex != INVALID_PARAMETER_INDEX)
        startFrom = stoi(argv[startFromIndex]);

    if (outImagesPrefixIndex != INVALID_PARAMETER_INDEX)
        outImagesPrefix = argv[outImagesPrefixIndex];

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

    cout << "Input image size: " << IMAGE_WIDTH << "x" << IMAGE_HEIGHT << endl;

    if (IMAGE_WIDTH % gridSize != 0 || IMAGE_HEIGHT % gridSize != 0)
        exitWithMessage(MESSAGE_WRONG_IMAGE_SIZE);

    const int COLS = IMAGE_WIDTH / gridSize;
    const int ROWS = IMAGE_HEIGHT / gridSize;

    // Keep track of the created image in order to avoid duplicate, if required
    struct CreatedImage {
        sf::Image *image;
        int number;
    } typedef CreatedImage;

    std::list<CreatedImage> outImageList;

    int incrementalIndex = startFrom;

    const int pixelArrayLength = gridSize * gridSize * 4;

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            int rowPixel = row * gridSize;
            int colPixel = col * gridSize;

            int boundInt = -1;

            cout << endl << "Inspecting image at [" << row << ", " << col << "]" << endl;

            unsigned int firstCellPixel = inputPixelMap.getPixel(colPixel, rowPixel).toInteger();
            firstCellPixel = (firstCellPixel >> 8); // Ignore alpha by taking just RGB components (first 24 bits)

            cout << "First pixel: " << hex << firstCellPixel << dec << endl;

            // Check if is bound
            for (std::list<Bound>::iterator iter = boundList.begin();
                iter != boundList.end();
                iter++) {
                if (firstCellPixel == (*iter).imageColor ) {
                    // Deterministic conversion
                    if ((*iter).mapIntCount == 1) {
                        boundInt = (*iter).mapInts[0];
                        cout << "Found a parameter bound for cell at [" << row << ", " << col
                             << "]. Using int {" << boundInt << "}" << endl;

                    }
                    // Random conversion
                    else {
                        int randomIndex = rand() % (*iter).mapIntCount;
                        boundInt = (*iter).mapInts[randomIndex];
                            cout << "Found a parameter bound for cell at [" << row << ", " << col
                                 << "]. Using random int {" << boundInt << "}" << endl;
                    }
                }
            }

            // No int is associated with this color => create a tile.
            if (boundInt == -1) {
                sf::Image *outImage = new sf::Image();
                outImage->create(gridSize, gridSize);
                outImage->copy(inputPixelMap, 0, 0, sf::IntRect(colPixel, rowPixel, gridSize, gridSize));

                int index = 0;

                // Check no duplicate
                if (noDuplicate) {
                    for (std::list<CreatedImage>::iterator iter = outImageList.begin();
                        iter != outImageList.end();
                        iter++) {
                        bool imagesAreEqual = true;

                        // Compare the images pixel-to-pixel
                        // Image should have the same size, skip the check

                        const unsigned char * alreadyCreatedImagePixels = (*iter).image->getPixelsPtr();
                        const unsigned char * imageToCreatePixels = outImage->getPixelsPtr();

                        for (int i = 0; i < pixelArrayLength && imagesAreEqual; i++)
                            imagesAreEqual = alreadyCreatedImagePixels[i] == imageToCreatePixels[i];

                        // Found an image equals to the new, avoid creation and bind
                        // it with the int used for that image
                        if (imagesAreEqual) {
                            boundInt = (*iter).number;
                            cout << "Cell at [" << row << ", " << col
                                 << "] is equals to previously created image. Recycling bound int {"
                                 << boundInt << "}" << endl;

                            break;
                        }
                    }
                }

                if (boundInt == -1) {
                    // No image equals to this one found, create the image and push it in the list.
                    cout << "Creating image at [" << row << ", " << col
                         << "] using new int {" << incrementalIndex << "}" << endl;

                    string outImageFilename(outImagesDir);
                    outImageFilename += "/";
                    outImageFilename += outImagesPrefix;
                    outImageFilename += to_string(incrementalIndex);
                    outImageFilename += ".png";

                    outImage->saveToFile(outImageFilename);

                    outImageList.push_front(CreatedImage { outImage, incrementalIndex });

                    boundInt = incrementalIndex;
                    incrementalIndex++;
                }
            }
            outTxtFile << boundInt << OUT_TXT_SEPARATOR;
        }
        outTxtFile << endl;
    }

    outTxtFile.close();

    cout << "Process completed. Created: " << incrementalIndex - startFrom << " images" << endl;
}
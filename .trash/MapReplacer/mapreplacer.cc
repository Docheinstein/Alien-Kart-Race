#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <list>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

/**
Logica:
Data un'immagine, la considera sezionata di una dimensione definita dall'utente (es 15x15)
e per ogni cella considera il primo pixel, se esso corrisponde ad uno specificato sostituisce
la cella con l'immagine associata.
*/

// Mandatory
const char * PARAMETER_INPUT_NAME = "-in";
const char * PARAMETER_OUT_NAME = "-out";
const char * PARAMETER_GRID_SIZE = "-grid-size";

// Optional
const char * PARAMETER_REPLACE = "-replace";
const char * PARAMETER_IN_IMAGES_DIR = "-in-images-dir";

const char * MESSAGE_INPUT_FILE_NOT_FOUND = "Error: input file not found.";
const char * MESSAGE_WRONG_PARAMETER_NUMBER = "Error: every mandatory parameter should be specified.";
const char * MESSAGE_FILE_CREATION_FAILED = "Error: file creation has failed.";
const char * MESSAGE_WRONG_IMAGE_SIZE =
"Error: image has an invalid size. \
Both width and height should be a multiple of the specified grid size.";
const char * MESSAGE_WRONG_TILESET_SIZE =
"Error: invalid tileset size. \
It should be a multiple of the specified grid size.";
const char * MESSAGE_TILESET_SIZE_NOT_ENOUGH =
"Error: tiles exceed the tileset size: use a smaller map or specify a greater size for the tileset";

const char * MESSAGE_HELP =
"--- Welcome to MapTilesetGenerator! ---\n\n\
MapTilesetGenerator is a tool for: \n\
1. Create a tileset by stripping the original image for the specified grid size.\n\
2. Create a raw map that contains the coordinate of the tile relative to the position on the tileset.\n\
 The created map has n rows and 2n cols, in which a cell is identified by a pair (row and col) that specifies\
 the position in pixel of the tile for the sub-image in the tileset.\n\n\
Valid parameters:\n\n\
MANDATORY \n\
-in <filename> \n\
  File name of the input image file. \n\
  (e.g. -in map.png) \n\n\
-out <filename> \n\
   File name of the output image file. \n\
   (e.g. -out map_out.png) \n\n\
-grid-size <int> \n\
  Size of a single cell of the input image. \n\
  (e.g. -grid-size 16) \n\n\
OPTIONAL \n\
-replace <color RGB string> <image> [<image> <image> ...]\n\
  Checks the first pixel of each cell of the specified grid size, if the color matches\
 the specified one, the given image is placed in the position of the cell \n\
  More than an image can be passed as parameter, in that case, the cell will be associated with\
 an image taken at random between the specified ones. \n\
  Note: image name can't start with '-' (reserved for parameters).\n\
  (e.g. -replace ff3ab5 12) \n\
  (e.g. -replace ff3ab5 12 13 15 17) \n\n\
-in-images-dir <path_without_final_slash>\n\
  The directory the input bound images will be taken from. \n\
  (e.g. -in-images-dir img)";


const int INVALID_PARAMETER_INDEX = -1;

void exitWithMessage(const char *message) {
    cout << message << endl;
    exit(-1);
}

void exitWithMessage(string messageStr) {
    exitWithMessage(messageStr.c_str());
}

struct ReplaceImage {
    const char *filename;
    sf::Image image;
};

struct ColorBindImage {
    unsigned int color;
    int imageCount;
    ReplaceImage *images;
} typedef Bound;

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        exitWithMessage(MESSAGE_HELP);
    }

    srand(time(NULL));

    int inImageIndex = INVALID_PARAMETER_INDEX;
    int outImageIndex = INVALID_PARAMETER_INDEX;
    int gridSizeIndex = INVALID_PARAMETER_INDEX;

    int inImagesDirIndex = INVALID_PARAMETER_INDEX;

    std::list<Bound> boundList;

    // Parse parameters
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], PARAMETER_INPUT_NAME) == 0) {
            if (++i < argc)
                inImageIndex = i;
            else {
                string wrongParamSyntax = PARAMETER_INPUT_NAME;
                wrongParamSyntax += " parameter has wrong syntax. Aborting.";
                exitWithMessage(wrongParamSyntax);
            }
        }
        else if (strcmp(argv[i], PARAMETER_OUT_NAME) == 0) {
            if (++i < argc)
                outImageIndex = i;
            else {
                string wrongParamSyntax = PARAMETER_OUT_NAME;
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
        else if (strcmp(argv[i], PARAMETER_REPLACE) == 0) {
            if (i + 2 < argc) {
                i++;

                unsigned int imageColor;
                std::stringstream colorHexStr;
                colorHexStr << std::hex << argv[i];
                colorHexStr >> imageColor;

                // Fill an array with the image filenames after this parameter
                int imageCount = 0;

                i++;

                // Count the ints
                int j = i;
                while (j < argc && argv[j][0] != '-' /* End of command or next parameter found */) {
                    imageCount++;
                    j++;
                }

                ReplaceImage *ris = new ReplaceImage[imageCount];

                // Fill with the images
                j = 0;
                while (j < imageCount) {
                    ris[j].filename = argv[i + j]; ;
                    j++;
                }

                boundList.push_front(Bound { imageColor, imageCount, ris });
            }
            else {
                string wrongParamSyntax = PARAMETER_REPLACE;
                wrongParamSyntax += " parameter has wrong syntax. Ignoring.";
                cout << wrongParamSyntax;
            }
        }
        else if (strcmp(argv[i], PARAMETER_IN_IMAGES_DIR) == 0) {
            if (++i < argc)
                inImagesDirIndex = i;
            else {
                string wrongParamSyntax = PARAMETER_IN_IMAGES_DIR;
                wrongParamSyntax += " parameter has wrong syntax. Ignoring.";
                cout << wrongParamSyntax;
            }
        }
    }

    if (inImageIndex == INVALID_PARAMETER_INDEX ||
        outImageIndex == INVALID_PARAMETER_INDEX ||
        gridSizeIndex == INVALID_PARAMETER_INDEX) {
        exitWithMessage(MESSAGE_WRONG_PARAMETER_NUMBER);
    }


    // Mandatory
    const char *inImageFilename = argv[inImageIndex];
    const char *outImageFilename = argv[outImageIndex];
    const int gridSize = stoi(argv[gridSizeIndex]);

    // Optional
    const char *inImagesDir = "./";

    if (inImagesDirIndex != INVALID_PARAMETER_INDEX)
        inImagesDir = argv[inImagesDirIndex];


    // Preload replacements
    for (std::list<Bound>::iterator iter = boundList.begin();
        iter != boundList.end();
        iter++) {
        cout << "Color [" << hex << (*iter).color << "] will be converted to image(s) [ ";

        for (int i = 0; i < (*iter).imageCount; i++) {
            cout << dec << (*iter).images[i].filename << " ";

            // Preload replacement
            std::string inImageName = inImagesDir;
            inImageName += "/";
            inImageName += (*iter).images[i].filename;
            (*iter).images[i].image.loadFromFile(inImageName);
        }

        cout << "]" << endl;
    }

    // Start image parsing
    cout << "----------------------------------------------" << endl
         << "------" << "Starting image parsing" << "------" << endl
         << "----------------------------------------------" << endl;

    // Load input image
    sf::Image inputPixelMap;

    if (!inputPixelMap.loadFromFile(inImageFilename))
        exitWithMessage(MESSAGE_INPUT_FILE_NOT_FOUND);

    sf::Vector2u imageSize = inputPixelMap.getSize();
    const int IMAGE_WIDTH = imageSize.x;
    const int IMAGE_HEIGHT = imageSize.y;

    cout << "Input image size: " << IMAGE_WIDTH << "x" << IMAGE_HEIGHT << endl;

    if (IMAGE_WIDTH % gridSize != 0 || IMAGE_HEIGHT % gridSize != 0)
        exitWithMessage(MESSAGE_WRONG_IMAGE_SIZE);

    const int COLS = IMAGE_WIDTH / gridSize;
    const int ROWS = IMAGE_HEIGHT / gridSize;


    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {

            int rowPixel = row * gridSize;
            int colPixel = col * gridSize;

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
                    if ((*iter).imageCount == 1) {
                        cout << "Found a parameter bound for cell at [" << row << ", " << col << "]" << endl;
                        inputPixelMap.copy((*iter).images[0].image, colPixel, rowPixel, sf::IntRect(0, 0, 15, 15));
                    }
                    // Random conversion
                    else {
                        cout << "Found a random parameter bound for cell at [" << row << ", " << col << "]" << endl;
                        int randomIndex = rand() % (*iter).imageCount;
                        inputPixelMap.copy((*iter).images[randomIndex].image, colPixel, rowPixel, sf::IntRect(0, 0, 15, 15));
                    }
                }
            }
        }
    }

    inputPixelMap.saveToFile(outImageFilename);

    cout << "Process completed." << endl;
}
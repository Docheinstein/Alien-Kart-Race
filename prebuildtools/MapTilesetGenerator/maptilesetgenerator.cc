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
const char * PARAMETER_OUT_RAW_NAME = "-out-raw";
const char * PARAMETER_OUT_TILESET_NAME = "-out-tileset";
const char * PARAMETER_GRID_SIZE = "-grid-size";
const char * PARAMETER_TILESET_SIZE = "-tileset-size";

// Optional
const char * PARAMETER_BIND = "-bind";
const char * PARAMETER_NO_DUPLICATE = "-no-duplicate";
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
2. Create a raw map that contains the coordinate of the tile relative to the position on the tileset.\n\n\
Valid parameters:\n\n\
MANDATORY \n\
-in <filename> \n\
  File name of the input image file. \n\
  (e.g. -inname map.png) \n\n\
-out-raw <filename> \n\
  File name of the output txt file. \n\
  (e.g. -outname map.txt) \n\n\
-out-tileset <filename> \n\
  File name of the output txt file. \n\
  (e.g. -outname map.txt) \n\n\
-grid-size <int> \n\
  Size of a single cell of the input image. \n\
  (e.g. -grid-size 16) \n\n\
-tileset-size <int>\n\
  Specifies the size of the tileset.\n\
  The size of the tileset must be a multiple of the grid size.\n\
  The tileset is filled starting from the first row, in horizontal;\
 when the first row is filled, the second will be used, and so on...\n\
  (e.g. -tileset-size 512)\n\n\
OPTIONAL \n\
-bind <color RGB string> <image> [<image> <image> ...]\n\
  Checks the first pixel of each cell of the specified grid size, if the color matches\
 the specified one, the given image is used for the cell and doesn't create an image for that cell in the tileset. \n\
  More than an image can be passed as parameter, in that case, the cell will be associated with\
 an image taken at random between the specified ones. \n\
  Note: image name can't start with '-' (reserved for parameters).\n\
  (e.g. -bind ff3ab5 12) \n\
  (e.g. -bind ff3ab5 12 13 15 17) \n\n\
-in-images-dir <path_without_final_slash>\n\
  The directory the input bound images will be taken from. \n\
  (e.g. -in-images-dir img)\n\n\
-no-duplicate \n\
  If a parsed cell image is equals to a previosuly created image, doesn't create the image and instead\
 use the tileset coordinate associated with the old cell for this cell too. (Can slow down the process, but the result is optimized). \n\
  (e.g. -no-duplicate)";


const int INVALID_PARAMETER_INDEX = -1;

const char OUT_RAW_SEPARATOR = ' ';

void exitWithMessage(const char *message) {
    cout << message << endl;
    exit(-1);
}

void exitWithMessage(string messageStr) {
    exitWithMessage(messageStr.c_str());
}

struct TilesetImage {
    sf::Image *image;
    int row;
    int col;
} typedef TilesetImage;

struct BoundImage {
    TilesetImage tilesetImage;
    const char *filename;
} typedef BoundImage;

struct ColorBindImage {
    unsigned int color;
    int imageCount;
    BoundImage *images;
} typedef Bound;

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        exitWithMessage(MESSAGE_HELP);
    }

    srand(time(NULL));

    int inImageIndex = INVALID_PARAMETER_INDEX;
    int outRawIndex = INVALID_PARAMETER_INDEX;
    int outTilesetIndex = INVALID_PARAMETER_INDEX;
    int gridSizeIndex = INVALID_PARAMETER_INDEX;
    int tilesetSizeIndex = INVALID_PARAMETER_INDEX;

    int inImagesDirIndex = INVALID_PARAMETER_INDEX;

    bool noDuplicate = false;

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
        else if (strcmp(argv[i], PARAMETER_OUT_RAW_NAME) == 0) {
            if (++i < argc)
                outRawIndex = i;
            else {
                string wrongParamSyntax = PARAMETER_OUT_RAW_NAME;
                wrongParamSyntax += " parameter has wrong syntax. Aborting.";
                exitWithMessage(wrongParamSyntax);
            }
        }
        else if (strcmp(argv[i], PARAMETER_OUT_TILESET_NAME) == 0) {
            if (++i < argc)
                outTilesetIndex = i;
            else {
                string wrongParamSyntax = PARAMETER_OUT_TILESET_NAME;
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
        else if (strcmp(argv[i], PARAMETER_TILESET_SIZE) == 0) {
            if (++i < argc)
                tilesetSizeIndex = i;
            else {
                string wrongParamSyntax = PARAMETER_TILESET_SIZE;
                wrongParamSyntax += " parameter has wrong syntax. Aborting.";
                exitWithMessage(wrongParamSyntax);
            }
        }
        else if (strcmp(argv[i], PARAMETER_BIND) == 0) {
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

                BoundImage *boundImages = new BoundImage[imageCount];

                // Fill with the ints
                j = 0;
                while (j < imageCount) {
                    boundImages[j].filename = argv[i + j];
                    j++;
                }

                boundList.push_front(Bound { imageColor, imageCount, boundImages });
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
        outRawIndex == INVALID_PARAMETER_INDEX ||
        outTilesetIndex == INVALID_PARAMETER_INDEX ||
        gridSizeIndex == INVALID_PARAMETER_INDEX ||
        tilesetSizeIndex == INVALID_PARAMETER_INDEX) {
        exitWithMessage(MESSAGE_WRONG_PARAMETER_NUMBER);
    }

    for (std::list<Bound>::iterator iter = boundList.begin();
        iter != boundList.end();
        iter++) {
        cout << "Color [" << hex << (*iter).color << "] will be converted to image(s) [ ";

        for (int i = 0; i < (*iter).imageCount; i++) {
            cout << dec << (*iter).images[i].filename << " ";
        }

        cout << "]" << endl;
    }

    // Start image parsing
    cout << "----------------------------------------------" << endl
         << "------" << "Starting image parsing" << "------" << endl
         << "----------------------------------------------" << endl;

    // Mandatory
    const char *inImageFilename = argv[inImageIndex];
    const char *outRawFilename = argv[outRawIndex];
    const char *outTilesetFilename = argv[outTilesetIndex];
    const int gridSize = stoi(argv[gridSizeIndex]);
    const int tilesetSize = stoi(argv[tilesetSizeIndex]);

    // Optional
    const char *inImagesDir = "./";

    if (inImagesDirIndex != INVALID_PARAMETER_INDEX)
        inImagesDir = argv[inImagesDirIndex];

    // Check tileset size
    if (tilesetSize % gridSize != 0)
        exitWithMessage(MESSAGE_WRONG_TILESET_SIZE);

    // Load input image
    sf::Image inputPixelMap;

    if (!inputPixelMap.loadFromFile(inImageFilename))
        exitWithMessage(MESSAGE_INPUT_FILE_NOT_FOUND);

    // Create output txt file
    ofstream outRawFile(outRawFilename);
    if (!outRawFile)
        exitWithMessage(MESSAGE_FILE_CREATION_FAILED);

    sf::Vector2u imageSize = inputPixelMap.getSize();
    const int IMAGE_WIDTH = imageSize.x;
    const int IMAGE_HEIGHT = imageSize.y;

    cout << "Input image size: " << IMAGE_WIDTH << "x" << IMAGE_HEIGHT << endl;

    if (IMAGE_WIDTH % gridSize != 0 || IMAGE_HEIGHT % gridSize != 0)
        exitWithMessage(MESSAGE_WRONG_IMAGE_SIZE);

    const int COLS = IMAGE_WIDTH / gridSize;
    const int ROWS = IMAGE_HEIGHT / gridSize;

    sf::Image tilesetImage;
    tilesetImage.create(tilesetSize, tilesetSize);

    // First of all load the input images and place them in the tileset

    int tilesetCurrentColPixel = 0;
    int tilesetCurrentRowPixel = 0;

    // Keep track of the created image in order to avoid duplicate, if required
    // and keeps the coordinate of the image on the tileset.
    std::list<TilesetImage> tilesetImageList;

    for (std::list<Bound>::iterator iter = boundList.begin();
        iter != boundList.end();
        iter++) {
        for (int i = 0; i < (*iter).imageCount; i++) {
            sf::Image *inImage = new sf::Image();
            std::string inImageName = inImagesDir;
            inImageName += "/";
            inImageName += (*iter).images[i].filename;
            inImage->loadFromFile(inImageName);

            tilesetImage.copy(*inImage, tilesetCurrentColPixel, tilesetCurrentRowPixel, sf::IntRect(0, 0, gridSize, gridSize));

            (*iter).images[i].tilesetImage.image = inImage;
            (*iter).images[i].tilesetImage.row = tilesetCurrentRowPixel;
            (*iter).images[i].tilesetImage.col = tilesetCurrentColPixel;

            // Keep track of the bound tile in the main tile list
            tilesetImageList.push_back((*iter).images[i].tilesetImage);

            tilesetCurrentColPixel += gridSize;

            // Row finished, put them in next row
            if (tilesetCurrentColPixel >= tilesetSize) {
                tilesetCurrentColPixel = 0;
                tilesetCurrentRowPixel += gridSize;
            }
        }
    }

    const int pixelArrayLength = gridSize * gridSize * 4;

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {

            int rowPixel = row * gridSize;
            int colPixel = col * gridSize;

            // int boundInt = -1;
            int boundTilesetRow = -1;
            int boundTilesetCol = -1;

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
                        boundTilesetRow = (*iter).images[0].tilesetImage.row;
                        boundTilesetCol = (*iter).images[0].tilesetImage.col;
                        cout << "Found a parameter bound for cell at [" << row << ", " << col
                             << "]. Using tile at {" << boundTilesetRow << ", " << boundTilesetCol << "}" << endl;

                    }
                    // Random conversion
                    else {
                        int randomIndex = rand() % (*iter).imageCount;
                        boundTilesetRow = (*iter).images[randomIndex].tilesetImage.row;
                        boundTilesetCol = (*iter).images[randomIndex].tilesetImage.col;
                        cout << "Found a parameter bound for cell at [" << row << ", " << col
                             << "]. Using tile at {" << boundTilesetRow << ", " << boundTilesetCol << "}" << endl;
                    }
                }
            }

            // No image is associated with this color => copy the image in the tileset.
            if (boundTilesetRow == -1 || boundTilesetCol == -1) {
                sf::Image *cellImage = new sf::Image();
                cellImage->create(gridSize, gridSize);
                cellImage->copy(inputPixelMap, 0, 0, sf::IntRect(colPixel, rowPixel, gridSize, gridSize));

                // Check no duplicate
                if (noDuplicate) {
                    for (std::list<TilesetImage>::iterator iter = tilesetImageList.begin();
                        iter != tilesetImageList.end();
                        iter++) {
                        bool imagesAreEqual = true;

                        // Compare the images pixel-to-pixel
                        // Image should have the same size, skip the check

                        const unsigned char * alreadyCreatedImagePixels = (*iter).image->getPixelsPtr();
                        const unsigned char * imageToCreatePixels = cellImage->getPixelsPtr();

                        for (int i = 0; i < pixelArrayLength && imagesAreEqual; i++)
                            imagesAreEqual = alreadyCreatedImagePixels[i] == imageToCreatePixels[i];

                        // Found a tile equals to the new cell, avoid re-adding it to the tileset and bind
                        // it with the position used for that tile
                        if (imagesAreEqual) {
                            boundTilesetRow = (*iter).row;
                            boundTilesetCol = (*iter).col;

                            cout << "Cell at [" << row << ", " << col
                                 << "] is equals to previously created image. Recycling tile at {"
                                 << boundTilesetRow << ", " << boundTilesetCol << "}" << endl;

                            break;
                        }
                    }
                }


                if (boundTilesetRow == -1 || boundTilesetCol == -1) {
                    boundTilesetRow = tilesetCurrentRowPixel;
                    boundTilesetCol = tilesetCurrentColPixel;
                    // No image equals to this one found, create the image and push it in the list.
                    cout << "Adding image to tileset at [" << row << ", " << col
                         << "] using coordinate {" << tilesetCurrentRowPixel << ", " << tilesetCurrentColPixel << "}" << endl;;

                    tilesetImage.copy(
                        *cellImage, tilesetCurrentColPixel, tilesetCurrentRowPixel,
                        sf::IntRect(0, 0, gridSize, gridSize));

                    tilesetImageList.push_front(TilesetImage { cellImage, tilesetCurrentRowPixel, tilesetCurrentColPixel });

                    tilesetCurrentColPixel += gridSize;

                    // Row finished, put them in next row
                    if (tilesetCurrentColPixel >= tilesetSize) {
                        tilesetCurrentColPixel = 0;
                        tilesetCurrentRowPixel += gridSize;

                        if (tilesetCurrentRowPixel >= tilesetSize) {
                            outRawFile.close();
                            exitWithMessage(MESSAGE_TILESET_SIZE_NOT_ENOUGH);
                        }
                    }
                }
            }
            outRawFile << dec << boundTilesetRow << OUT_RAW_SEPARATOR << boundTilesetCol << OUT_RAW_SEPARATOR;
        }
        outRawFile << endl;
    }

    outRawFile.close();

    tilesetImage.saveToFile(outTilesetFilename);

    cout << "Process completed. Tiles: " << tilesetImageList.size() << endl;
}
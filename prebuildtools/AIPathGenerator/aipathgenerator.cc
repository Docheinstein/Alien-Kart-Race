#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <list>
#include <cstdlib>
#include <cmath>

using namespace std;

/**
Logica:
Data un'immagine, la considera sezionata di una dimensione definita dall'utente (es 15x15).
Successivamente costruisce un path i cui i vertici sono le celle alla posizione in cui il
primo pixel è un colore specificato dall'utente.
Per definire un path è necessario specificare almeno il colore del 1 e 2 punto (in modo che
la direzione sia definita) e successivamente il colore di tutti gli altri punti.
*/

// Mandatory
const char * PARAMETER_INPUT_IMAGE_NAME = "-in";
const char * PARAMETER_GRID_SIZE = "-size";

// Optional
const char * PARAMETER_PATH = "-path";

const char * MESSAGE_INPUT_FILE_NOT_FOUND = "Error: input file not found.";
const char * MESSAGE_WRONG_PARAMETER_NUMBER = "Error: every mandatory parameter should be specified.";
const char * MESSAGE_FILE_CREATION_FAILED = "Error: file creation has failed.";
const char * MESSAGE_WRONG_IMAGE_SIZE =
"Error: image has an invalid size. \
Both width and height should be a multiple of the specified grid size.";

const char * MESSAGE_HELP =
"--- Welcome to AIPathGenerator! ---\n\n\
MinimapGenerator is a tool for: \n\
1. Create a path (succession of points) from an image in which each cell have the specified grid size.\n\
The output is in the from (row col row col) in which each tuple specify a point of the path.\n\
In order to detect the direction of a path, the first two points of it should be specified.\n\
The next points are calculated by choosing the minimum distance from the previous point.\n\
Valid parameters:\n\n\
MANDATORY \n\
-in <filename> \n\
  File name of the input image file. \n\
  (e.g. -in map.png) \n\n\
-size <int> \n\
  Size of a single cell of the input image. \n\
  (e.g. -size 16) \n\n\
OPTIONAL \n\
-path <filename> <first RGB color> <second RGB color> [<other RGB color> <other RGB color> ...] \n\
  Create a path in a raw format with the given filename. \n\
  First and second color must be specified and have to be unique in the image, in order to detect\
  the begin of the path and its direction. \n\
  The other points are calculated by choosing the minimum distance from the previous point.\n\
  (e.g. -path ai_path_1.txt f7a6b4 f536b3 bf7812)";

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
    int gridSizeIndex = INVALID_PARAMETER_INDEX;

    struct PathParam {
        const char *filename;
        int colorCount;;
        unsigned int *colors;
    } typedef PathParam;

    struct PathPoint {
        int row;
        int col;
        unsigned int color;
    } typedef PathPoint;

    struct Path {
        PathParam param;
        std::list<PathPoint> points;
    } typedef Path;

    std::list<Path> pathList;

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
        else if (strcmp(argv[i], PARAMETER_GRID_SIZE) == 0) {
            if (++i < argc)
                gridSizeIndex = i;
            else {
                string wrongParamSyntax = PARAMETER_GRID_SIZE;
                wrongParamSyntax += " parameter has wrong syntax. Aborting.";
                exitWithMessage(wrongParamSyntax);
            }
        }
        else if (strcmp(argv[i], PARAMETER_PATH) == 0) {
            if (i < argc + 3) {
                Path path;

                i++;

                const char *pathOutName = argv[i];

                // Fill an array with the colors after this parameter
                int colorCount = 0;

                i++;

                // Count the colors
                int j = i;
                while (j < argc && argv[j][0] != '-' /* End of command or next parameter found */) {
                    colorCount++;
                    j++;
                }

                unsigned int *pathColors = new unsigned int[colorCount];

                // Fill with the colors
                j = 0;
                while (j < colorCount) {

                    unsigned int imageColor;
                    std::stringstream colorHexStr;
                    colorHexStr << std::hex << argv[i + j];
                    colorHexStr >> imageColor;

                    pathColors[j] = imageColor;
                    j++;
                }

                path.param.filename = pathOutName;
                path.param.colorCount = colorCount;
                path.param.colors = pathColors;

                pathList.push_front(path);
            }
        }
    }

    if (inImageIndex == INVALID_PARAMETER_INDEX ||
        gridSizeIndex == INVALID_PARAMETER_INDEX) {
        exitWithMessage(MESSAGE_WRONG_PARAMETER_NUMBER);
    }

    for (std::list<Path>::iterator iter = pathList.begin();
        iter != pathList.end();
        iter++) {
        PathParam &pp = (*iter).param;

        cout << "Path [" << hex << pp.filename << "] has the following colors" << endl;;

        cout << "-First color: " << hex << pp.colors[0] << endl;
        cout << "-Second color: " << hex << pp.colors[1] << endl;
        cout << "Other colors: " << endl;
        for (int i = 2; i < pp.colorCount; i++) {
            cout << hex << pp.colors[i] << " ";
        }

        cout << endl << endl;
    }

    // Start image parsing
    cout << "----------------------------------------------" << endl
         << "------" << "Starting image parsing" << "------" << endl
         << "----------------------------------------------" << endl;

    // Mandatory
    const char *inImageFilename = argv[inImageIndex];
    const int gridSize = stoi(argv[gridSizeIndex]);

    // Load input image
    sf::Image inputPixelMap;

    if (!inputPixelMap.loadFromFile(inImageFilename))
        exitWithMessage(MESSAGE_INPUT_FILE_NOT_FOUND);

    sf::Vector2u imageSize = inputPixelMap.getSize();
    const int IMAGE_WIDTH = imageSize.x;
    const int IMAGE_HEIGHT = imageSize.y;

    cout << "Input image size: " << dec << IMAGE_WIDTH << "x" << IMAGE_HEIGHT << endl;

    if (IMAGE_WIDTH % gridSize != 0 || IMAGE_HEIGHT % gridSize != 0)
        exitWithMessage(MESSAGE_WRONG_IMAGE_SIZE);

    const int COLS = IMAGE_WIDTH / gridSize;
    const int ROWS = IMAGE_HEIGHT / gridSize;

    // First find the points on the image and add them to the paths.
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            int rowPixel = row * gridSize;
            int colPixel = col * gridSize;

            int boundInt = NON_BOUND_CELL_DEFAULT_VALUE;

            // cout << endl << "Inspecting image at [" << row << ", " << col << "]" << endl;

            unsigned int firstCellPixel = inputPixelMap.getPixel(colPixel, rowPixel).toInteger();
            firstCellPixel = (firstCellPixel >> 8); // Ignore alpha by taking just RGB components (first 24 bits)
            firstCellPixel &= 0xffffff;

            // cout << "First pixel: " << hex << firstCellPixel << dec << endl;

            int pathIndex = 0;

            for (std::list<Path>::iterator iter = pathList.begin();
                iter != pathList.end();
                iter++, pathIndex++) {

                Path &p = (*iter);
                PathParam &pp = p.param;

                for (int i = 0; i < pp.colorCount; i++) {
                    if (firstCellPixel == pp.colors[i] ) {
                        // Found a point, add it to the list of points

                        cout << "Found a point with color [" << hex << firstCellPixel << dec
                             << "] for the path [" << pathIndex << "] at [" << row << ", " << col
                             << "]" << endl;
                        p.points.push_back(PathPoint {row, col, firstCellPixel});
                    }
                }
            }
        }
    }

    int pathIndex = 0;

    for (std::list<Path>::iterator iter = pathList.begin();
        iter != pathList.end();
        iter++, pathIndex++) {

        std::list<PathPoint> orderedPath;
        Path &p = (*iter);

        // Create output txt file
        ofstream outPathFile(p.param.filename);
        if (!outPathFile) {
            cout << "Warning. Can't create file with name: " << p.param.filename << ", skipping path." << endl;
            continue;
        }

        cout << "Points of path [" << pathIndex << "]" << endl;

        for (std::list<PathPoint>::iterator pointsIter = p.points.begin();
            pointsIter != p.points.end();
            pointsIter++) {

            PathPoint &point = (*pointsIter);
            cout << dec << "(" << point.row << ", " << point.col << ") with color [" << hex << point.color << "]"<< endl;
        }

        // First of all append the first and the second point
        for (std::list<PathPoint>::iterator pointsIter = p.points.begin();
            pointsIter != p.points.end();
            pointsIter++) {

            PathPoint &point = (*pointsIter);
            // Check if this is the first point
            if (point.color == p.param.colors[0]) {
                orderedPath.push_front(point);
                pointsIter = p.points.erase(pointsIter);
            }
            // Check if this is the second point
            else if (point.color == p.param.colors[1]) {
                orderedPath.push_back(point);
                pointsIter = p.points.erase(pointsIter);
            }
        }

        // Logic: find the closest point to the last appended point, that has not been considered yet
        // and add it to the list in order.
        PathPoint lastPoint { orderedPath.back() };

        while (p.points.size() > 0) {
            // Set the current minimum as infinite
            int minimumDistance = std::numeric_limits<int>::max();

            std::list<PathPoint>::iterator nearestPointIter;

            for (std::list<PathPoint>::iterator pointsIter = p.points.begin();
                pointsIter != p.points.end();
                pointsIter++) {

                PathPoint &point = (*pointsIter);

                int lastRow = lastPoint.row;
                int lastCol = lastPoint.col;

                int rowDiff = lastPoint.row - point.row;
                int colDiff = lastPoint.col - point.col;

                int distanceBetweenPoints = sqrt(rowDiff * rowDiff + colDiff * colDiff);

                if (distanceBetweenPoints < minimumDistance) {
                    minimumDistance = distanceBetweenPoints;
                    nearestPointIter = pointsIter;
                }
            }

            PathPoint nearestPointCopy = *nearestPointIter;
            lastPoint = nearestPointCopy;

            // Add the point to the ordered and remove from the list of points to consider
            orderedPath.push_back(nearestPointCopy);
            p.points.erase(nearestPointIter);
        }

        int pointIndex = 0;
        cout << "Ordered path [" << pathIndex << "]" << endl;
        for (std::list<PathPoint>::iterator iter = orderedPath.begin();
            iter != orderedPath.end();
            iter++, pointIndex++) {

            PathPoint &point = (*iter);
            cout <<  dec << "[" << pointIndex << "] (" << point.row << ", " << point.col << ") with color [" << hex << point.color << "]"<< endl;

            outPathFile << dec << point.row << " " << point.col << " ";
        }
        outPathFile.close();
    }

    cout << "Process completed." << endl;
}
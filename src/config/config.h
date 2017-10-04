#ifndef CONFIG_H
#define CONFIG_H

/** Contains global configuration constants shared between entities. */
namespace Config {
    /** Width of the game window. */
    const int WINDOW_WIDTH = 640;

    /** Height of the game window. */
    const int WINDOW_HEIGHT = 480;

    /** Updates per second the game should run at. */
    const int TARGET_UPDATES_PER_SECOND = 60;

    /** Size of the minimap. */
    const int MINIMAP_SIZE = Config::WINDOW_WIDTH * 0.4;
}

#endif // CONFIG_H
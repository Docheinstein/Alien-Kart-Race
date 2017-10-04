#ifndef CONFIG_H
#define CONFIG_H

/** Contains global configuration constants shared between entities. */
namespace Config {
    /** Width of the game window. */
    extern const int WINDOW_WIDTH;

    /** Height of the game window. */
    extern const int WINDOW_HEIGHT;

    /** Updates per second the game should run at. */
    extern const int TARGET_UPDATES_PER_SECOND;

    /** Size of the minimap. */
    extern const int MINIMAP_SIZE;
}

#endif // CONFIG_H
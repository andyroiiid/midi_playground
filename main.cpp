#include <SDL.h>

#include "app.h"

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);

    try {
        App app;
        app.mainLoop();
    } catch (RtMidiError &error) {
        error.printMessage();
    }

    SDL_Quit();
    return 0;
}

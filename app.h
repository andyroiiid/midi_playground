//
// Created by andyroiiid on 12/25/2021.
//

#ifndef MIDI_PLAYGROUND_APP_H
#define MIDI_PLAYGROUND_APP_H

#include <SDL_render.h>
#include <RtMidi.h>

class App {
public:
    App();

    App(const App &) = delete;

    App &operator=(const App &) = delete;

    ~App();

    void mainLoop();

private:
    void tick();

    void processEvents();

    void processMidiMessages();

    void processMidiMessage(std::vector<uint8_t> &message);

    void draw();

    SDL_Window *_window = nullptr;
    SDL_Renderer *_renderer = nullptr;
    bool _running = false;

    RtMidiIn _midi;

    uint8_t _keys[128]{0};
};

#endif //MIDI_PLAYGROUND_APP_H

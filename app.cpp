//
// Created by andyroiiid on 12/25/2021.
//

#include "app.h"

#include <SDL_events.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>
#include <fmt/format.h>

App::App() {
    _window = SDL_CreateWindow(
            "midi playground",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            800,
            600,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED
    );

    _renderer = SDL_CreateRenderer(
            _window,
            -1,
            SDL_RENDERER_PRESENTVSYNC
    );

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().IniFilename = nullptr;
    ImGui_ImplSDL2_InitForSDLRenderer(_window);
    ImGui_ImplSDLRenderer_Init(_renderer);
}

App::~App() {
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
}

void App::mainLoop() {
    unsigned int nPorts = _midi.getPortCount();
    for (int i = 0; i < nPorts; i++) {
        std::cout << _midi.getPortName(i) << std::endl;
    }

    _midi.openPort(0);
    _midi.ignoreTypes(false, false, false);

    _running = true;
    while (_running) {
        tick();
    }
}

void App::tick() {
    processEvents();
    processMidiMessages();
    draw();
}

void App::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            _running = false;
        }
    }
}

void App::processMidiMessages() {
    std::vector<uint8_t> message;
    do {
        _midi.getMessage(&message);
        if (message.empty()) break;
        processMidiMessage(message);
    } while (true);
}

void App::processMidiMessage(std::vector<uint8_t> &message) {
    // https://users.cs.cf.ac.uk/Dave.Marshall/Multimedia/node158.html
    uint8_t statusByte = message[0];
    switch (statusByte >> 4) {
        case 0x8:
        case 0x9:
            _keys[message[1]] = message[2];
            break;
        case 0xA:
            fmt::print("polyphonic key pressure {}\n", fmt::join(message, ", "));
            break;
        case 0xB:
            fmt::print("control change {}\n", fmt::join(message, ", "));
            break;
        case 0xC:
            fmt::print("program change {}\n", fmt::join(message, ", "));
            break;
        case 0xD:
            fmt::print("channel pressure {}\n", fmt::join(message, ", "));
            break;
        case 0xE:
            fmt::print("pitch bend {}\n", fmt::join(message, ", "));
            break;
        case 0xF: //ignore system messages
        default:
            break;
    }
}

void App::draw() {
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame(_window);
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Midi Playground", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

    for (int note = 0; note < 128; note++) {
        int octave = note / 12 - 1;
        static const char *names[12] = {
                "C",
                "C#/Db",
                "D",
                "D#/Eb",
                "E",
                "F",
                "F#/Gb",
                "G",
                "G#/Ab",
                "A",
                "A#/Bb",
                "B"
        };
        ImGui::LabelText(
                fmt::format("{}{}", names[note % 12], octave).c_str(),
                "%d",
                _keys[note]
        );
    }

    ImGui::End();

    ImGui::Render();

    SDL_RenderClear(_renderer);
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(_renderer);
}

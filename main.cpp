#include <SDL.h>
#include <RtMidi.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>
#include <fmt/format.h>

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(
            "midi playground",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            800,
            600,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED
    );
    SDL_Renderer *renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_PRESENTVSYNC
    );

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().IniFilename = nullptr;
    ImGui_ImplSDL2_InitForSDLRenderer(window);
    ImGui_ImplSDLRenderer_Init(renderer);

    try {
        RtMidiIn midiIn;

        unsigned int nPorts = midiIn.getPortCount();
        for (int i = 0; i < nPorts; i++) {
            std::cout << midiIn.getPortName(i) << std::endl;
        }

        midiIn.openPort(0);

        midiIn.ignoreTypes(false, false, false);

        std::vector<uint8_t> messageBuffer;

        bool running = true;
        while (running) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                ImGui_ImplSDL2_ProcessEvent(&event);
                if (event.type == SDL_QUIT) {
                    running = false;
                }
            }

            do {
                midiIn.getMessage(&messageBuffer);
                if (messageBuffer.empty()) break;
                // ignore timing message
                if (messageBuffer[0] != 0xF8) {
                    fmt::print("message = {:02X}\n", fmt::join(messageBuffer, ", "));
                }
            } while (true);

            ImGui_ImplSDLRenderer_NewFrame();
            ImGui_ImplSDL2_NewFrame(window);
            ImGui::NewFrame();

            ImGui::ShowDemoWindow();

            ImGui::Render();

            SDL_RenderClear(renderer);
            ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
            SDL_RenderPresent(renderer);
        }
    } catch (RtMidiError &error) {
        error.printMessage();
    }

    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

#ifndef RENDERER_BASIC_RENDERER
#define RENDERER_BASIC_RENDERER

#include <Renderer/renderer_basic.hpp>
#include <windows.h>

#ifdef INCLUDE_CPP
    #include <Renderer/RendererBasic/rb_poll.cpp>
    #include <Renderer/RendererBasic/rb_interactive.cpp>
    #include <Renderer/RendererBasic/rb_command_com.cpp>
    #include <Renderer/RendererBasic/rb_command_proc.cpp>
    #include <Renderer/RendererBasic/rb_render_game.cpp>
    #include <Renderer/RendererBasic/rb_render_gui.cpp>
    #include <Renderer/RendererBasic/rb_draw_basic.cpp>
    #include <Renderer/RendererBasic/rb_sprite.cpp>
#endif


namespace SupDef {

    RendererBasic::RendererBasic() { }
    
    RendererBasic::~RendererBasic() {
        ImGui::SFML::Shutdown();
    }

    void RendererBasic::start() {
        window = std::make_unique<sf::RenderWindow>(sf::VideoMode({width, height}), "Supreme", sf::State::Windowed);

        HWND hwnd = window->getNativeHandle();
        ShowWindow(hwnd, SW_MAXIMIZE);

        auto initOk = ImGui::SFML::Init(*(window.get()));

        gameView = window->getDefaultView();
        guiView  = window->getDefaultView();

        width  = window->getSize().x;
        height = window->getSize().y;
        dispatch<WindowResizeEvent>(width, height);
        subscribeToEvents();
    }

    void RendererBasic::end() {
        if(window->isOpen()) window->close();
    }
    
    bool RendererBasic::draw() {
        if(!window->isOpen()) { return false; }

        ImGui::SFML::Update(*(window.get()), deltaClock.restart());
        window->clear(sf::Color::Black);

        window->setView(gameView);
        doScrolling();
        renderGame();

        window->setView(guiView);
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);

        ImGui::Begin("GameOverlay", nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoMove |
            // ImGuiWindowFlags_NoInputs |
            ImGuiWindowFlags_NoBringToFrontOnFocus);

        renderGui();
        ImGui::End();
        ImGui::SFML::Render(*(window.get()));

        updateCommand();

        window->display();
        dispatch<WindowResizeEvent>(window->getSize().x, window->getSize().y);
        return true;
    }

}

#endif

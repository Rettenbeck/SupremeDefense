#pragma once

#include <Renderer/renderer_basic.hpp>
#include <Renderer/RendererBasic/rb_poll.cpp>
#include <Renderer/RendererBasic/rb_interactive.cpp>
#include <Renderer/RendererBasic/rb_command_com.cpp>
#include <Renderer/RendererBasic/rb_command_proc.cpp>
#include <Renderer/RendererBasic/rb_render_game.cpp>
#include <Renderer/RendererBasic/rb_render_gui.cpp>
#include <Renderer/RendererBasic/rb_draw_basic.cpp>


namespace SupDef {

    RendererBasic::RendererBasic() { }
    
    RendererBasic::~RendererBasic() {
        ImGui::SFML::Shutdown();
    }

    void RendererBasic::start() {
        unsigned w, h;
        w = width;
        h = height;
        window = std::make_unique<sf::RenderWindow>(sf::VideoMode({width, height}), "Supreme");
        auto initOk = ImGui::SFML::Init(*(window.get()));

        gameView = window->getDefaultView();
        guiView  = window->getDefaultView();

        //font = std::make_unique<sf::Font>();
        //font->loadFromFile("arial.ttf");
        assert(globalDispatcher);
        globalDispatcher->dispatch<WindowResizeEvent>(width, height);
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
        ImGui::Begin("GameOverlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);
        renderGui();
        ImGui::End();
        ImGui::SFML::Render(*(window.get()));

        updateCommand();

        window->display();
        globalDispatcher->dispatch<WindowResizeEvent>(window->getSize().x, window->getSize().y);
        return true;
    }

}
    
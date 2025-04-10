#pragma once

#include <Renderer/renderer_basic.hpp>


namespace SupDef {

    bool RendererBasic::poll() {
        assert(globalDispatcher);
        if(!window->isOpen()) { return false; }
        prepareSelectableList();

        while (const std::optional<sf::Event> event = window->pollEvent()) {
            if(event.has_value()) { ImGui::SFML::ProcessEvent(*(window.get()), event.value()); }

            if(event->is<sf::Event::Closed>()) {
                window->close();
                return false;
            }

            if(event.has_value()) {
                if (const auto* resizedEvent = event->getIf<sf::Event::Resized>()) {
                    globalDispatcher->dispatch<WindowResizeEvent>(resizedEvent->size.x, resizedEvent->size.y);
                }

                if (const auto* scrollEvent = event->getIf<sf::Event::MouseWheelScrolled>()) {
                    if (scrollEvent->delta > 0)
                        zoom(0.9f);
                    else
                        zoom(1.1f);
                }
                
                if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyEvent->code == sf::Keyboard::Key::Left)  keyL = true;
                    if (keyEvent->code == sf::Keyboard::Key::Right) keyR = true;
                    if (keyEvent->code == sf::Keyboard::Key::Up)    keyU = true;
                    if (keyEvent->code == sf::Keyboard::Key::Down)  keyD = true;
                    if (keyEvent->code == sf::Keyboard::Key::D) debugMode = !debugMode;
                    if (keyEvent->code == sf::Keyboard::Key::T) drawTiles = !drawTiles;
                    if (keyEvent->code == sf::Keyboard::Key::S) {
                        std::string filename = "state";
                        std::string ext = ".txt";
                        std::string prev = filename + ext;
                        game->serialize(prev);
                        for (int i = 0; i < 2; i++) {
                            std::stringstream ss;
                            auto g = std::make_unique<SupDef::Game>();
                            g->deserialize(prev);
                            ss << filename << (i+2) << ext;
                            prev = ss.str();
                            g->serialize(prev); 
                        }
                    }
                    if (keyEvent->code == sf::Keyboard::Key::K) {
                        game->serialize("state_secondary.txt");
                    }
                    if (keyEvent->code == sf::Keyboard::Key::L) {
                        game->deserialize("state.txt");
                    }
                    if (keyEvent->code == sf::Keyboard::Key::P) {
                        auto currentPlayer = game->getThisPlayer()->id;
                        auto otherPlayer = game->otherPlayer;
                        game->otherPlayer = currentPlayer;
                        game->setThisPlayer(otherPlayer);
                    }
                    if (keyEvent->code == sf::Keyboard::Key::Y) {
                        globalDispatcher->dispatch<StartNetworkGameAsServerEvent>();
                    }
                    if (keyEvent->code == sf::Keyboard::Key::X) {
                        globalDispatcher->dispatch<StartNetworkGameAsClientEvent>();
                    }
                    if (keyEvent->code == sf::Keyboard::Key::C) {
                        globalDispatcher->dispatch<CompleteServerEvent>();
                    }
                    if (keyEvent->code == sf::Keyboard::Key::M) {
                        globalDispatcher->dispatch<StartTestGameEvent>();
                    }
                }

                if (const auto* keyEvent = event->getIf<sf::Event::KeyReleased>()) {
                    if (keyEvent->code == sf::Keyboard::Key::Left)  keyL = false;
                    if (keyEvent->code == sf::Keyboard::Key::Right) keyR = false;
                    if (keyEvent->code == sf::Keyboard::Key::Up)    keyU = false;
                    if (keyEvent->code == sf::Keyboard::Key::Down)  keyD = false;
                }

                if (const auto* mouseClickEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    // mousePos = mouseClickEvent->position;
                    if (mouseClickEvent->button == sf::Mouse::Button::Left ) onMouseClick(MLEFT );
                    if (mouseClickEvent->button == sf::Mouse::Button::Right) onMouseClick(MRIGHT);
                }
            }
        }

        return true;
    }

}

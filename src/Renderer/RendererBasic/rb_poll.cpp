#include <Renderer/renderer_basic.hpp>

#pragma once


namespace SupDef {

    bool RendererBasic::poll() {
        if(!window->isOpen()) { return false; }

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
                    if (keyEvent->code == sf::Keyboard::Key::N) trigger(1);
                    if (keyEvent->code == sf::Keyboard::Key::M) trigger(2);
                }

                if (const auto* keyEvent = event->getIf<sf::Event::KeyReleased>()) {
                    if (keyEvent->code == sf::Keyboard::Key::Left)  keyL = false;
                    if (keyEvent->code == sf::Keyboard::Key::Right) keyR = false;
                    if (keyEvent->code == sf::Keyboard::Key::Up)    keyU = false;
                    if (keyEvent->code == sf::Keyboard::Key::Down)  keyD = false;
                }

                // if (const auto* mouseMoveEvent = event->getIf<sf::Event::MouseMoved>()) {
                //     mousePos = mouseMoveEvent->position;
                // }

                if (const auto* mouseClickEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    // mousePos = mouseClickEvent->position;
                    if (mouseClickEvent->button == sf::Mouse::Button::Left ) onMouseClick(true );
                    if (mouseClickEvent->button == sf::Mouse::Button::Right) onMouseClick(false);
                }
            }
        }

        return true;
    }

}

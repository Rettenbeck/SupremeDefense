#include <Renderer/renderer_basic.hpp>

#pragma once


namespace SupDef {

    void RendererBasic::updateMove() {
        float moveAmountCur = moveAmount * currentZoom;
        if(keyL) move(-moveAmountCur, 0);
        if(keyR) move( moveAmountCur, 0);
        if(keyU) move(0, -moveAmountCur);
        if(keyD) move(0,  moveAmountCur);
    }

    void RendererBasic::zoom(float factor) {
        sf::Vector2f mouseWorldBefore = window->mapPixelToCoords(sf::Mouse::getPosition(*window.get()), gameView);
        gameView.zoom(factor);
        sf::Vector2f mouseWorldAfter = window->mapPixelToCoords(sf::Mouse::getPosition(*window.get()), gameView);
        sf::Vector2f movement = mouseWorldBefore - mouseWorldAfter;
        gameView.move(movement);
        currentZoom *= factor;
    }

    void RendererBasic::move(float offsetX, float offsetY) {
        sf::Vector2f offset(offsetX, offsetY);
        gameView.move(offset);
    }

    void RendererBasic::onMouseClick(bool left) {
        json j;
        auto pos = getMousePosWorld();
        j["x"] = pos.x;
        j["y"] = pos.y;
        globalDispatcher->dispatch<ConfirmCommandEvent>(left, j);
    }

    sf::Vector2i RendererBasic::getMousePos() {
        return sf::Mouse::getPosition(*window.get());
    }

    sf::Vector2f RendererBasic::getMousePosWorld() {
        return window->mapPixelToCoords(getMousePos(), gameView);
    }

}

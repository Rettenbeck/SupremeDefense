#include <Renderer/renderer_basic.hpp>

#pragma once


namespace SupDef {

    void RendererBasic::drawRect(float x, float y, float width, float height, ColorData cData) {
        sf::RectangleShape rectangle(sf::Vector2f(width, height));
        rectangle.setFillColor(cData.fillColor);
        rectangle.setOutlineColor(cData.outlineColor);
        rectangle.setOutlineThickness(cData.outlineThickness);
        rectangle.setPosition(sf::Vector2f(x, y));
        window->draw(rectangle);
    }
    
    void RendererBasic::drawCircle(float x, float y, float r, ColorData cData) {
        sf::CircleShape circle(r);
        circle.setPointCount(20);
        circle.setFillColor(cData.fillColor);
        circle.setOutlineColor(cData.outlineColor);
        circle.setOutlineThickness(cData.outlineThickness);
        circle.setPosition(sf::Vector2f(x - r, y - r));
        window->draw(circle);
    }

    void RendererBasic::drawLine(float x1, float y1, float x2, float y2, sf::Color color) {
        //
    }

}

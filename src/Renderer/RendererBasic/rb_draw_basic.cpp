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
        auto w = x2 - x1; if (w < 1.0) w = 1.0;
        auto h = y2 - y1; if (h < 1.0) h = 1.0;
        ColorData cr(color, color, 0);
        drawRect(x1, y1, w, h, cr);
    }

    void RendererBasic::drawLine1D(float x1, float y1, float x2, float y2, sf::Color color) {
        sf::VertexArray line(sf::PrimitiveType::Lines, 2);
        line[0].position = sf::Vector2f(x1, y1);
        line[0].color = color;
        line[1].position = sf::Vector2f(x2, y1);
        line[1].color = color;
        window->draw(line);
    }

    void RendererBasic::drawSelection(float x, float y, float width, float height, ColorData cData) {
        int cl = 5, ol = cData.outlineThickness;  // Marker length, outline thickness
        int longSide = cl + 2 * ol;
        int shortSide = 1 + 2 * ol;
        int x1 = x, y1 = y, x2 = x + width - 1, y2 = y + height - 1;
        ColorData cr(cData.outlineColor, cData.outlineColor, 0);
        auto c = cData.fillColor;

        // Top left
        drawRect(x1 - ol, y1 - ol, longSide, shortSide, cr);
        drawRect(x1 - ol, y1 - ol, shortSide, longSide, cr);
        drawLine(x1, y1, x1 + cl, y1, c);
        drawLine(x1, y1, x1, y1 + cl, c);

        // Top right
        drawRect(x2 - ol - cl + 1, y1 - ol, longSide, shortSide, cr);
        drawRect(x2 - ol, y1 - ol, shortSide, longSide, cr);
        drawLine(x2 - cl + 1, y1, x2, y1, c);
        drawLine(x2, y1, x2, y1 + cl, c);

        // Bottom left
        drawRect(x1 - ol, y2 - ol, longSide, shortSide, cr);
        drawRect(x1 - ol, y2 - ol - cl + 1, shortSide, longSide, cr);
        drawLine(x1, y2, x1 + cl, y2, c);
        drawLine(x1, y2 - cl + 1, x1, y2, c);

        // Bottom right
        drawRect(x2 - ol - cl + 1, y2 - ol, longSide, shortSide, cr);
        drawRect(x2 - ol, y2 - ol - cl + 1, shortSide, longSide, cr);
        drawLine(x2 - cl + 1, y2, x2, y2, c);
        drawLine(x2, y2 - cl + 1, x2, y2 + 1, c);
    }

}

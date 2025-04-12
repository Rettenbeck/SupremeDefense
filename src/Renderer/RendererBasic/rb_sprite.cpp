#ifndef RENDERER_BASIC_SPRITE
#define RENDERER_BASIC_SPRITE

#include <Renderer/renderer_basic.hpp>


namespace SupDef {

    void RendererBasic::drawSprites(EntityManager* entityManager) {
        assert(entityManager);
        auto graphics = entityManager->getEntitiesWithComponents<PositionComponent, GraphicComponent>();

        for (auto& [entity, pos, gra] : graphics) {
            gra->zBuffer = (pos->y / 16);
        }
        std::sort(graphics.begin(), graphics.end(), [](const auto& a, const auto& b) {
            return (std::get<2>(a))->zBuffer < (std::get<2>(b))->zBuffer;
        });

        for (auto& [entity, pos, gra] : graphics) {
            drawSprite(entity, pos, gra);
        }
    }

    void RendererBasic::drawSprite(Entity* entity, PositionComponent* pos, GraphicComponent* gra) {
        assert(entity);
        auto col = entity->getComponent<CollisionComponent>();
        drawSprite(pos, col, gra);
    }

    void RendererBasic::drawSprite(PositionComponent* pos, CollisionComponent* col, GraphicComponent* gra) {
        assert(pos); assert(gra);
        float x = pos->x;
        float y = pos->y;
        auto texture = getTexture(gra);
        if (!texture) return;
        if (gra->drawCentered) {
            if (col) {
                auto center = col->getCenter(pos->x, pos->y);
                x = center.x - texture->getSize().x / 2;
                y = center.y - texture->getSize().y / 2;
            }
        }
        sf::Sprite sprite(*texture);
        sprite.setPosition(sf::Vector2f(x, y));
        window->draw(sprite);
    }

    sf::Texture* RendererBasic::getTexture(GraphicComponent* graphic) {
        assert(graphic);
        return getTexture(graphic->filepath);
    }

    sf::Texture* RendererBasic::getTexture(const std::string& filename) {
        auto it = textureMap.find(filename);
        if (it != textureMap.end()) return (it->second).get();
    
        auto texture = std::make_unique<sf::Texture>();
        if (!texture->loadFromFile(filename)) LOG_ERROR("Graphics path not found: &1", filename)
        auto ptr = texture.get();
        textureMap[filename] = std::move(texture);
        return ptr;
    }

}

#endif

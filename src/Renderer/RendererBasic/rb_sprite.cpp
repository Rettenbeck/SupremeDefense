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
        assert(entity); assert(pos); assert(gra);
        auto texture = getTexture(gra);
        if (!texture) return;

        auto col = entity->getComponent<CollisionComponent>();
        auto map = entity->getComponent<MapComponent>();
        float x = pos->x;
        float y = pos->y;
        if (col) {
            auto center = col->getCenter(pos->x, pos->y);
            x = center.x;
            y = center.y;
        } else if (map) {
            x += map->width  / 2;
            y += map->height / 2;
        }

        sf::Sprite sprite(*texture);
        handleAnimation(entity, sprite);
        auto bounds = sprite.getLocalBounds();
        sprite.setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
        sprite.setPosition(sf::Vector2f(x, y));
        if (gra->rotate) handleRotation(entity, sprite, pos, gra->drawCentered);
        window->draw(sprite);
    }

    void RendererBasic::handleAnimation(Entity* entity, sf::Sprite& sprite) {
        auto ani = entity->getComponent<AnimationComponent>();
        if (!ani) return;

        int frame = ani->currentFrame / ani->animationSpeed;
        int cx = (frame % ani->columnCount) * ani->width ;
        int cy = (frame / ani->columnCount) * ani->height;
        sprite.setTextureRect(sf::IntRect(sf::Vector2i(cx, cy), sf::Vector2i(ani->width, ani->height)));

        ani->currentFrame++;
        if (ani->currentFrame / ani->animationSpeed >= ani->totalFrames) {
            ani->currentFrame = 0;
            if (ani->dieAfterAnimation) {
                assert(globalDispatcher);
                globalDispatcher->queueEvent<DieAfterAnimationEvent>(entity->id);
            }
        }
    }

    void RendererBasic::handleRotation(Entity* entity, sf::Sprite& sprite, PositionComponent* pos, bool centered) {
        auto mov = entity->getComponent<MovementComponent>();
        if (!mov) return;
        
        auto radians = pos->getAngleOfVelocity();
        if (radians == -1.0) {
            radians = pos->lastAngle;
            if (radians == -1.0) return;
        }

        auto angle = sf::radians(radians);
        sprite.setRotation(angle);
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

#include <ECS/entity.hpp>
#include <EventDispatcher/include.hpp>

#pragma once


namespace SupDef {

    class SelectionManager : public Listener {
        private:
            EntityIDs selected;
        
        public:
            void selectUnit(Entity* entity) {
                if (!entity) return;
                if (entity->hasComponent<SelectableComponent>()) {
                    selected.push_back(entity->id);
                }
            }
        
            void deselectAll() {
                selected.clear();
            }
        
            const EntityIDs& getSelectedUnits() const {
                return selected;
            }

            void initialize() {
                globalDispatcher->subscribe<UnitSelectedEvent>([this](const SupDef::Events& events) {
                    selected.clear();
                    for (const auto& event : events) {
                        const auto& typedEvent = static_cast<const SupDef::UnitSelectedEvent&>(*event);
                        selectUnit(typedEvent.entity);
                    }
                });
            }

            void setGlobalDispatcher(EventDispatcher* globalDispatcher_) { globalDispatcher = globalDispatcher_; }

    };

    using USelectionManager = std::unique_ptr<SelectionManager>;

}

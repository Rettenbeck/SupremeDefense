#pragma once

#include <ECS/entity.hpp>
#include <EventDispatcher/include.hpp>


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
                SUBSCRIBE_HEAD_BEGIN(globalDispatcher, UnitSelectedEvent)
                    selected.clear();
                    SUBSCRIBE_BODY_BEGIN
                        selectUnit(typedEvent.entity);
                    SUBSCRIBE_BODY_END
                SUBSCRIBE_HEAD_END
            }

            void setGlobalDispatcher(EventDispatcher* globalDispatcher_) { globalDispatcher = globalDispatcher_; }

    };

    using USelectionManager = std::unique_ptr<SelectionManager>;

}

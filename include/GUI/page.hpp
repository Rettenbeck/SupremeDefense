#pragma once

#include <GUI/gui_manager.hpp>


namespace SupDef {

    class Page : public Listener {
        protected:
            using CreatePageFunc    = std::function<std::unique_ptr<Page>()>;
            using CreatePageFuncMap = std::unordered_map<int, CreatePageFunc>;

            GuiManager* guiManager = nullptr;
            Game* game = nullptr;
            SelectionManager* selectionManager = nullptr;
            SocketBackend* socketBackend = nullptr;

            bool isOnTop = false;
            bool isBlocked = false;     // Blocked by higher pages -> may be drawn but does not react
            bool isCovered = false;     // Covered by higher pages -> will not be drawn

            PageId pageId = -1;
            PageTypeId pageTypeId = -1;

            static CreatePageFuncMap& registry() {
                static CreatePageFuncMap instance;
                return instance;
            }
        
        public:
            Page(PageTypeId pageTypeId_) { pageTypeId = pageTypeId_; }

            virtual void initialize() = 0;

            virtual void build() = 0;

            virtual bool isBlocking() = 0;
            virtual bool isCovering() = 0;

            PageTypeId getPageTypeId() { return pageTypeId; }

            void gotoPage(PageTypeId pageTypeId) {
                dispatch<GotoPageEvent>(pageTypeId);
            }

            void pushPage(PageTypeId pageTypeId) {
                dispatch<PushPageEvent>(pageTypeId);
            }

            void close() {
                uint32_t id = pageId;
                dispatch<ClosePageEvent>(id);
            }

            void setOnTop(bool isOnTop_) { isOnTop = isOnTop_; }
            void setBlocked(bool isBlocked_) { isBlocked = isBlocked_; }
            void setCovered(bool isCovered_) { isCovered = isCovered_; }

            bool getBlocked() { return isBlocked; }
            bool getCovered() { return isCovered; }

            template<typename TypeElement, typename... Args>
            TypeElement* addElement(Args&&... args) {
                assert(guiManager);
                auto ptr = guiManager->add(std::make_unique<TypeElement>(std::forward<Args>(args)...));
                return dynamic_cast<TypeElement*>(ptr);
            }

            template<typename TypeElement, typename TypeEvent, typename... Args>
            TypeElement* addClickable(Args&&... args) {
                assert(guiManager);
                auto ptr = guiManager->addClickable<TypeEvent>(std::make_unique<TypeElement>(std::forward<Args>(args)...));
                return dynamic_cast<TypeElement*>(ptr);
            }

            template<typename TypeElement, typename TypeEvent, class... A1, class... A2>
            TypeElement* addClickableEvent(std::tuple<A1...> a1, std::tuple<A2...> a2) {
                assert(guiManager);

                auto ptr = guiManager->addClickable(
                    std::apply([](auto&&... x){
                        return std::make_unique<TypeElement>(std::forward<decltype(x)>(x)...);
                    }, std::move(a1)),
                    [this, args = std::move(a2)]() mutable {
                        std::apply([this](auto&... xs){ dispatch<TypeEvent>(xs...); }, args);
                    }
                );
                return dynamic_cast<TypeElement*>(ptr);
            }

            template<typename TypeElement, typename... Args>
            TypeElement* addClickableCB(GuiMemberFunc func, Args&&... args) {
                assert(guiManager);
                auto ptr = guiManager->addClickable(std::make_unique<TypeElement>(std::forward<Args>(args)...), func);
                return dynamic_cast<TypeElement*>(ptr);
            }

            template<typename TypeEvent, typename... Args>
            GuiButton* addButton(Args&&... args) {
                return addClickable<GuiButton, TypeEvent>(std::forward<Args>(args)...);
            }

            template<typename TypeEvent, class... A1, class... A2>
            GuiButton* addButtonEvent(std::tuple<A1...> a1, std::tuple<A2...> a2) {
                return addClickableEvent<GuiButton, TypeEvent>(a1, a2);
            }

            template<typename... Args>
            GuiButton* addButtonCB(GuiMemberFunc func, Args&&... args) {
                return addClickableCB<GuiButton>(func, std::forward<Args>(args)...);
            }

            void addElementToRow(GuiElementRow& row, GuiElement* element) {
                assert(element);
                element->embedded = true;
                row.push_back(element);
            }

            void finalizeSimpleTable(GuiTable* table) {
                assert(table);
                table->rows.clear();

                for(auto line : table->strData) {
                    GuiElementRow row;
                    for(auto str : line) {
                        addElementToRow(row, addElement<GuiLabel>(str));
                    }
                    table->rows.push_back(row);
                }
                table->distributeGuiIds();
            }

            template <typename T>
            static void addToRegistry(PageTypeId pageTypeId) {
                Page::registerPage(pageTypeId, [pageTypeId]() { return std::make_unique<T>(pageTypeId); });
            }

            static void registerPage(const int pageType, CreatePageFunc func) {
                registry()[pageType] = func;
            }
        
            static std::unique_ptr<Page> createPage(const PageTypeId pageTypeId) {
                auto it = registry().find(pageTypeId);
                if (it != registry().end()) {
                    return it->second();
                }
                return nullptr;
            }

            PageId getPageId() { return pageId; }
            void setPageId(PageId pageId_) { pageId = pageId_; }
            void setGuiManager(GuiManager* guiManager_) { guiManager = guiManager_; }
            void setGame(Game* game_) { game = game_; }
            void setSelectionManager(SelectionManager* selectionManager_) { selectionManager = selectionManager_; }
            void setSocketBackend(SocketBackend* socketBackend_) { socketBackend = socketBackend_; }

    };

    using UPage = std::unique_ptr<Page>;
    using Pages = std::vector<UPage>;

}

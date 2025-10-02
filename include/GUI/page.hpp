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

            static CreatePageFuncMap& registry() {
                static CreatePageFuncMap instance;
                return instance;
            }
        
        public:
            Page(PageId pageId_) { pageId = pageId_; }

            virtual void initialize() = 0;

            virtual void build() = 0;

            virtual bool isBlocking() = 0;
            virtual bool isCovering() = 0;

            PageId getPageId() { return pageId; }

            void gotoPage(PageId pageId) {
                assert(globalDispatcher);
                dispatch<GotoPageEvent>(pageId);
            }

            void pushPage(PageId pageId) {
                assert(globalDispatcher);
                dispatch<PushPageEvent>(pageId);
            }

            void close() {
                assert(globalDispatcher);
                dispatch<ClosePageEvent>();
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

            template<typename... Args>
            GuiButton* addButtonCB(GuiMemberFunc func, Args&&... args) {
                return addClickableCB<GuiButton>(func, std::forward<Args>(args)...);
            }

            void setGuiManager(GuiManager* guiManager_) { guiManager = guiManager_; }

            void setGame(Game* game_) { game = game_; }
            void setSelectionManager(SelectionManager* selectionManager_) { selectionManager = selectionManager_; }
            void setSocketBackend(SocketBackend* socketBackend_) { socketBackend = socketBackend_; }

            template <typename T>
            static void addToRegistry(PageId pageId) {
                Page::registerPage(pageId, [pageId]() { return std::make_unique<T>(pageId); });
            }

            static void registerPage(const int pageType, CreatePageFunc func) {
                registry()[pageType] = func;
            }
        
            static std::unique_ptr<Page> createPage(const int pageType) {
                auto it = registry().find(pageType);
                if (it != registry().end()) {
                    return it->second();
                }
                return nullptr;
            }

    };

    using UPage = std::unique_ptr<Page>;
    using Pages = std::vector<UPage>;

}

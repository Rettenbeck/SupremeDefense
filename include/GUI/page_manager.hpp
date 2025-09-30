#pragma once

#include <GUI/pages.hpp>


namespace SupDef {

    class PageManager : public Listener {
        private:
            GuiManager* guiManager = nullptr;
            Pages pages, tempPages;
        
            Game* game = nullptr;
            SelectionManager* selectionManager = nullptr;
            SocketBackend* socketBackend = nullptr;

        public:
            PageManager() { registerPages(); }

            void initialize() {
                SUBSCRIBE_SIMPLE(globalDispatcher, GotoPageEvent, gotoPage(typedEvent.pageId));
                SUBSCRIBE_SIMPLE(globalDispatcher, PushPageEvent, pushPage(typedEvent.pageId));
                SUBSCRIBE_SIMPLE(globalDispatcher, ClosePageEvent, closePage());
                SUBSCRIBE_SIMPLE(globalDispatcher, PrintPagesEvent, pagesToStr());
            }

            void update(float deltaTime) {
                assert(guiManager);
                if (pages.empty()) createInitialPage();
                setPageObjects();
                updatePageHierarchy();
                buildPages();
            }

            void buildPages() {
                for(auto& page : pages) {
                    if (page->getCovered()) continue;
                    page->build();
                    if (page->getBlocked()) guiManager->setElementsUnreactive();
                }
                guiManager->add(std::make_unique<GuiLabel>(GuiElementStyle::Default, 10, 10, guiManager->info));
            }

            void updatePageHierarchy() {
                if (pages.size() == 0) return;
                pages[pages.size() - 1]->setOnTop(true);

                int bIndex = -1, cIndex = -1;
                for(int i = 0; i < pages.size(); i++) {
                    pages[i]->setBlocked(false);
                    pages[i]->setCovered(false);
                    if (pages[i]->isBlocking()) bIndex = i;
                    if (pages[i]->isCovering()) cIndex = i;
                }

                if (bIndex > 0) for(int i = 0; i < bIndex; i++) pages[i]->setBlocked(true);
                if (cIndex > 0) for(int i = 0; i < cIndex; i++) pages[i]->setCovered(true);
            }

            void setPageObjects() {
                for(auto& page : pages) {
                    page->setGuiManager(guiManager);
                    page->setGame(game);
                    page->setSelectionManager(selectionManager);
                    page->setSocketBackend(socketBackend);
                }
            }

            void createInitialPage() {
                pushPage(PAGE_ID_START);
            }

            void transferPages() {
                if (tempPages.empty()) return;
                pages.insert(
                    pages.end(),
                    std::make_move_iterator(tempPages.begin()),
                    std::make_move_iterator(tempPages.end())
                );
                tempPages.clear();
            }

            void gotoPage(PageId pageId) {
                pages.clear();
                pushPage(pageId);
            }

            void pushPage(PageId pageId) {
                auto newPage = Page::createPage(pageId);
                assert(newPage);
                newPage->setGlobalDispatcher(globalDispatcher);
                newPage->initialize();
                pages.push_back(std::move(newPage));
            }

            void closePage() {
                if (pages.empty()) return;
                pages.pop_back();
            }

            void pagesToStr() {
                std::stringstream ss;
                ss << "Pages total: " << pages.size() << "\n";
                for (int i = 0; i < pages.size(); i++) {
                    ss << "  Page " << i
                        << "; blocked? " << ((pages[i]->getBlocked()) ? "yes" : "no")
                        << "; covered? " << ((pages[i]->getCovered()) ? "yes" : "no")
                        << "\n";
                }
                std::cout << ss.str();
            }

            void setGuiManager(GuiManager* guiManager_) { guiManager = guiManager_; }

            void setGame(Game* game_) { game = game_; }
            void setSelectionManager(SelectionManager* selectionManager_) { selectionManager = selectionManager_; }
            void setSocketBackend(SocketBackend* socketBackend_) { socketBackend = socketBackend_; }

            // Game* getGame() { return game; }
            // SelectionManager* getSelectionManager() { return selectionManager; }
            // SocketBackend* getSocketBackend() { return socketBackend; }
            
    };

    using UPageManager = std::unique_ptr<PageManager>;

}

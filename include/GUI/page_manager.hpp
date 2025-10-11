#pragma once

#include <GUI/pages.hpp>


namespace SupDef {

    class PageManager : public Listener {
        private:
            GuiManager* guiManager = nullptr;
            Pages pages, tempPages;
            PageId nextPageId = 1;
        
            Game* game = nullptr;
            SelectionManager* selectionManager = nullptr;
            SocketBackend* socketBackend = nullptr;
            Services* services = nullptr;

        public:
            PageManager() { registerPages(); }

            void initialize() {
                SUBSCRIBE(GotoPageEvent);
                SUBSCRIBE(PushPageEvent);
                SUBSCRIBE(ClosePageEvent);
                SUBSCRIBE(PrintPagesEvent);
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
                guiManager->add(std::make_unique<GuiLabel>(10, 10, guiManager->info));
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
                pushPage(PAGE_TYPE_ID_START);
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

            void pushPage(PageTypeId pageTypeId) {
                auto id = nextPageId++;
                auto newPage = Page::createPage(pageTypeId);
                assert(newPage);
                newPage->setPageId(id);
                newPage->setGlobalDispatcher(globalDispatcher);
                newPage->setServices(services);
                newPage->initialize();
                pages.push_back(std::move(newPage));
            }

            DEFINE_EVENT_CALLBACK(GotoPageEvent) {
                pages.clear();
                pushPage(event.pageTypeId);
            }

            DEFINE_EVENT_CALLBACK(PushPageEvent) {
                pushPage(event.pageTypeId);
            }

            DEFINE_EVENT_CALLBACK(ClosePageEvent) {
                if (pages.empty()) return;
                auto id = event.pageId;
                pages.erase(
                    std::remove_if(pages.begin(), pages.end(),
                                [id](const UPage& page) {
                                    return page->getPageId() == id;
                                }),
                    pages.end());
                pages.pop_back();
            }

            DEFINE_EVENT_CALLBACK(PrintPagesEvent) {
                std::stringstream ss;
                ss << "Pages total: " << pages.size() << "\n";
                for (int i = 0; i < pages.size(); i++) {
                    ss << "  Page " << i << "; id: " << pages[i]->getPageId()
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
            void setServices(Services* services_) { services = services_; }

    };

    using UPageManager = std::unique_ptr<PageManager>;

}

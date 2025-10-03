#define INCLUDE_CPP

#include <iostream>
#include <include.hpp>


void test_app();
void start_app();
auto app = std::make_unique<SupDef::App>();


int main() {
    start_app();
    app->run();
    
    return 0;
}


void start_app() {
    auto networkLayer = std::make_unique<SupDef::NetworkLayer>();
    auto replayLayer  = std::make_unique<SupDef::ReplayLayer>();
    auto actionRouter = std::make_unique<SupDef::ActionRouter>();
    auto renderLayer  = std::make_unique<SupDef::RenderLayer>();
    auto guiLayer     = std::make_unique<SupDef::GuiLayer>();

    app->addLayer(std::move(replayLayer));
    app->addLayer(std::move(actionRouter));
    app->addLayer(std::move(guiLayer));
    app->addLayer(std::move(renderLayer));
    app->addLayer(std::move(networkLayer));
}

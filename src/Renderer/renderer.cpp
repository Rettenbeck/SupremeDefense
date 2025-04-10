#pragma once

#include <Renderer/renderer.hpp>


namespace SupDef {

    Renderer::Renderer() { }
    Renderer::~Renderer() { }
    // void Renderer::initialize() { }
    void Renderer::start() { }
    void Renderer::end() { }
    bool Renderer::poll() { return false; }
    bool Renderer::draw() { return false; }

}

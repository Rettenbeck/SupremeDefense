#include <Util/util.hpp>

#pragma once


namespace SupDef {
    
    struct BoundingBox {
        bool isDefined = false;
        float x, y, w, h;

        BoundingBox() {}
        BoundingBox(float x_, float y_, float w_, float h_) : x(x_), y(y_), w(w_), h(h_), isDefined(true) {}

        void add(float x_, float y_, float w_, float h_) {
            if(isDefined) {
                float newX = std::min(x, x_);
                float newY = std::min(y, y_);
                w = std::max(x + w, x_ + w_) - newX;
                h = std::max(y + h, y_ + h_) - newY;
                x = newX; y = newY;
            } else {
                isDefined = true;
                x = x_; y = y_; w = w_; h = h_;
            }
        }

        void add(BoundingBox boundingBox) {
            if(!boundingBox.isDefined) return;
            add(boundingBox.x, boundingBox.y, boundingBox.w, boundingBox.h);
        }

    };

}
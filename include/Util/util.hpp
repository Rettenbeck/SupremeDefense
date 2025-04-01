#include <Util/basic.hpp>

#pragma once

namespace SupDef {

    static const double PI  = 3.14159265359;
    static const double TAU = 6.28318530718;

    class Math {
        public:

            inline static float getRadius(float x, float y) {
                return sqrt(x * x + y * y);
            }

            inline static double getTheta(float x, float y) {
                double theta;
                if(x == 0) {
                    if(y > 0) theta = PI / 2;
                    else theta = 3 * PI / 4;
                } else {
                    theta = atan(y / x);
                    if(x < 0) theta += PI;
                    if(x > 0 && y < 0) theta += TAU;
                }

                return theta;
            }

            inline static double flipX(double theta) {
                if(theta > PI) theta += (PI - theta) * 2;
                else theta -= (theta - PI) * 2;
                return theta;
            }

            inline static double getX(float a, float r) { return r * cos(a); }
            inline static double getY(float a, float r) { return r * sin(a); }

            inline static float getDistanceSquared(float x1, float y1, float x2, float y2) {
                auto dx = x1 - x2;
                auto dy = y1 - y2;
                return dx * dx + dy * dy;
            }

            inline static float getDistance(float x1, float y1, float x2, float y2) {
                return sqrt(getDistanceSquared(x1, y1, x2, y2));
            }

            inline static bool circleOverlap(float x1, float y1, float x2, float y2, float r1, float r2) {
                return (r1*r1 + 2*r1*r2 + r2*r2) >= getDistanceSquared(x1, y1, x2, y2);
            }

            inline static int getGridIndex(float value, int grid_size) {
                if(grid_size == 0) return value;
                return value / grid_size;
            }

            inline static int getGridPosition(float value, int grid_size) {
                if(grid_size == 0) return value;
                return getGridIndex(value, grid_size) * grid_size;
            }

            inline static std::tuple<float, float, float, float> getBoundingBoxFromCircle(float x, float y, float r) {
                return {x - r, y - r, r * 2.0, r * 2.0};
            }

    };

    struct V2 {
        int x = 0, y = 0;
        V2() { }
        V2(int x_, int y_) { x = x_; y = y_; }
    };

    using V2s = std::vector<V2>;

    struct VF2 {
        float x = 0, y = 0;
        VF2() { }
        VF2(float x_, float y_) { x = x_; y = y_; }
    };

    using VF2s = std::vector<VF2>;

    using Ints = std::vector<int>;
    
    template <typename T>
    void push_back_unique(std::vector<T>& vec, const T& value) {
        if (std::find(vec.begin(), vec.end(), value) == vec.end()) {
            vec.push_back(value);
        }
    }
    
    template <typename T>
    void remove_all(std::vector<T>& vec, const T& value) {
        vec.erase(std::remove(vec.begin(), vec.end(), value), vec.end());
    }
    
    template <typename T>
    bool remove_first(std::vector<T>& vec, const T& value) {
        auto it = std::find(vec.begin(), vec.end(), value);
        if (it != vec.end()) {
            vec.erase(it);
            return true;
        }
        return false;
    }

}

#include <Util/basic.hpp>
#include <App/Log/logger.hpp>

#pragma once


namespace SupDef {

    class Timer {
        public:
            using Clock = std::chrono::high_resolution_clock;
            using Duration = std::chrono::duration<double, std::milli>;
            
            Timer(size_t maxHistorySize = 60) 
                : maxHistorySize(maxHistorySize), lastMeasurement(0.0) {}

            void startMeasurement() {
                startTime = Clock::now();
            }

            void stopMeasurement() {
                auto endTime = Clock::now();
                lastMeasurement = std::chrono::duration_cast<Duration>(endTime - startTime).count();
                
                measurements.push_back(lastMeasurement);
                if (measurements.size() > maxHistorySize) {
                    measurements.pop_front();
                }
            }

            void waitUntilElapsed(double milliseconds) {
                auto elapsed = std::chrono::duration_cast<Duration>(Clock::now() - startTime).count();
                if (elapsed < milliseconds) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(milliseconds - elapsed)));
                }
            }
            
            double getLastMeasurement() const {
                return lastMeasurement;
            }

            const std::deque<double>& getMeasurements() const {
                return measurements;
            }

            void setMaxHistorySize(size_t newSize) {
                maxHistorySize = newSize;
                while (measurements.size() > maxHistorySize) {
                    measurements.pop_front();
                }
            }

            size_t getMaxHistorySize() const {
                return maxHistorySize;
            }

        private:
            Clock::time_point startTime;
            double lastMeasurement;
            std::deque<double> measurements;
            size_t maxHistorySize;
            
    };

}

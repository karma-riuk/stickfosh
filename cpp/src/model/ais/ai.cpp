#include "ai.hpp"

#include <thread>

Move ai::AI::search(const Board& b, bool am_white) {
    Move result;

    // Start computation in a separate thread
    std::thread computation_thread([&]() { result = _search(b, am_white); });

    // Start a timer thread to stop computation after given time
    std::thread timer_thread([&]() {
        std::this_thread::sleep_for(thinking_time);
        stop_computation = true;
    });

    // Wait for computation thread to finish
    computation_thread.join();

    // Ensure timer thread is also stopped
    timer_thread.join();
    return result;
}

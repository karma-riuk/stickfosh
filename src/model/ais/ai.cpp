#include "ai.hpp"

#include <condition_variable>
#include <ostream>
#include <thread>

static long int position_counter = 0;

Move ai::AI::search(const Board& b) {
    position_counter = 0;
    Move result;

    std::condition_variable cv;
    std::mutex cv_mutex;
    double elapsed;
    stop_computation = false;


    // Start computation in a separate thread
    std::thread computation_thread([&]() {
        auto start = std::chrono::steady_clock::now();
        result = _search(b);
        auto end = std::chrono::steady_clock::now();
        elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count();

        // Notify the timer thread that computation is done
        {
            std::lock_guard<std::mutex> lock(cv_mutex);
            stop_computation = true;
            cv.notify_one();
        }
    });

    // Start a timer thread to stop computation after given time
    std::thread timer_thread([&]() {
        std::unique_lock<std::mutex> lock(cv_mutex);
        if (!cv.wait_for(lock, thinking_time, [&] {
                return stop_computation.load();
            })) {
            // Timeout reached; set stop flag
            stop_computation = true;
        }
    });

    // Wait for computation thread to finish
    computation_thread.join();

    // Ensure timer thread is also stopped
    timer_thread.join();

    std::cout << "Took " << elapsed << " ms, " << "Looked at "
              << position_counter << " positions" << std::endl;
    return result;
}

int ai::AI::eval(const Board& b) {
    int ret = _eval(b);
    position_counter++;
    return ret;
}

#include <iostream>
#include <thread>             // For thread management
#include <mutex>              // For mutual exclusion
#include <condition_variable> // For thread synchronization using condition variables

// Shared Resources
int shared_counter = 0;           // Counter shared between threads.
std::mutex counter_mutex;         // Protects shared_counter from concurrent access.
std::condition_variable cv;       // Enables threads to communicate. One thread waits until another thread signals that a condition is met.
bool count_up_finished = false;   // Flag indicating if "count up" thread is done.

// Function for the "count up" thread
void countUp() {
    // Iterate from 0 to 20
    for (int i = 0; i <= 20; ++i) {
        // Acquire lock on the counter_mutex using std::lock_guard.
        {
            std::lock_guard<std::mutex> lock(counter_mutex);
            // Set shared_counter to current iteration
            shared_counter = i;
            // Print current count up value progress
            std::cout << "Count Up: " << shared_counter << std::endl;
        } // The std::lock_guard goes out of scope here, automatically releasing the lock.
        // Simulate work and thread communication using a pause.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Acquire lock again to update shared flag.
    {
        std::lock_guard<std::mutex> lock(counter_mutex);
        // Set flag to true indicating count up is complete
        count_up_finished = true;
        // Print thread counting up update message
        std::cout << "Count Up finished. Signalling Count Down thread." << std::endl;
    } // Lock is once again out of scope and automatically released.
    // Notify waiting "count down" thread that the condition (count_up_finished) has been met.
    cv.notify_one();
}

// Function for the "count down" thread
void countDown() {
    // Acquire unique lock on the counter_mutex.
    // std::unique_lock offers more control than std::lock_guard, allowing explicit unlock and relock.
    std::unique_lock<std::mutex> lock(counter_mutex);

    // Print "count down" thread waiting message
    std::cout << "Count Down thread waiting for Count Up to finish..." << std::endl;
    // Wait on 'cv'. This pauses the current thread and releases the lock until 'cv' is notified
    // and 'count_up_finished' is true.
    cv.wait(lock, [] { return count_up_finished; });
    // After 'count_up_finished' is true, the thread resumes, and the unique lock is reacquired.

    // Print starting count down value (which is the final value from the "count up" thread.)
    std::cout << "Count Down thread starting from " << shared_counter << std::endl;

    // Release the unique lock now. This lets other threads, like "count up", use the mutex during the count down.
    lock.unlock();

    // Iterate from shared_counter down to 0
    for (int i = shared_counter; i >= 0; --i) {
        // Acquire lock for each update to shared_counter within the loop, enabling atomic operations.
        {
            std::lock_guard<std::mutex> inner_lock(counter_mutex);
            // Set shared_counter to current iteration
            shared_counter = i;
            // Print current count down value progress
            std::cout << "Count Down: " << shared_counter << std::endl;
        } // The lock is automatically released here.
        // Simulate work using a pause.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    // Print thread counting down update message
    std::cout << "Count Down finished." << std::endl;
}

int main() {
    std::cout << "Program started." << std::endl;

    // Create two threads for "count up" and "count down" processes
    std::thread up_thread(countUp);
    std::thread down_thread(countDown);

    // Wait for both threads to complete
    up_thread.join();   // 'join()' pauses the main thread until 'up_thread' is done.
    down_thread.join(); // 'join()' pauses the main thread until 'down_thread' is done.

    // After both threads have finished, print update message with the final value of the shared_counter.
    std::cout << "Both threads finished. Final shared_counter: " << shared_counter << std::endl;
    std::cout << "Program finished." << std::endl;

    return 0; // Indicate successful program execution.
}
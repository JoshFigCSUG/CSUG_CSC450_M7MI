# CSUG_CSC450_M7MI
CSUG_CSC450_M7MI

This repository contains a C++ program focusing on concurrent programming using threads, mutexes, and condition variables. The project demonstrates a scenario where two threads manage a shared counter: one increments it to 20, and the other decrements it back to 0, with proper synchronization to ensure the second thread only starts after the first completes its task.

## Project Description

The core of this project is a C++ application that showcases inter-thread communication and synchronization. It involves a shared integer variable (`shared_counter`) accessed and modified by two distinct threads.

* **Count Up Thread**: This thread increments `shared_counter` from 0 to 20.
* **Count Down Thread**: This thread waits for the `shared_counter` to reach 20, then decrements it from 20 back to 0.

To prevent race conditions and ensure correct execution flow, `std::mutex` is used for mutual exclusion when accessing `shared_counter`, and `std::condition_variable` is employed to signal the "count down" thread when the "count up" thread has finished its task.

## Features

* **Concurrency**: Utilizes `std::thread` to create and manage two separate execution flows.
* **Shared Resource Management**: Employs `std::mutex` with `std::lock_guard` and `std::unique_lock` to protect the `shared_counter` variable from concurrent access.
* **Thread Synchronization**: Implements `std::condition_variable` to coordinate the start of the "count down" thread, ensuring it begins only after the "count up" thread completes its counting.
* **Output Monitoring**: Prints the progress of both counters to the console, allowing observation of their synchronized execution.

The program will output the counting progress of both threads, followed by a final message indicating the completion and the final value of the shared counter.

## Code Structure

* **`main` function**: The entry point of the program, responsible for creating and joining the `countUp` and `countDown` threads.
* **`countUp` function**:
    * Increments `shared_counter` from 0 to 20.
    * Uses `std::lock_guard` to protect `shared_counter` during updates.
    * Sets a `count_up_finished` flag to `true` upon completion.
    * Calls `cv.notify_one()` to signal the `countDown` thread.
* **`countDown` function**:
    * Uses `std::unique_lock` and `cv.wait()` to pause execution until `count_up_finished` is true.
    * Decrements `shared_counter` from its current value (which will be 20) down to 0.
    * Uses `std::lock_guard` within the loop to protect `shared_counter` during updates.

## Pseudocode

A `M7MI_pseudocode.txt` file is included in the repository, providing a high-level, language-agnostic representation of the program's logic. This can be helpful for understanding the flow of execution and synchronization mechanisms without diving into the specific C++ syntax immediately.

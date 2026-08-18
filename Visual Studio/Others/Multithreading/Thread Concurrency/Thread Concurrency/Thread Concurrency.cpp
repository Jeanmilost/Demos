/*******************************************************************************
 * ==> Thread Concurrency Example ---------------------------------------------*
 *******************************************************************************
 * Description: This demo demonstrates data race issues in thread concurrency  *
 *              and how synchronization tools prevent corruption. It compares: *
 *              1. std::atomic: Protects shared variables via hardware-level,  *
 *                 uninterrupted steps without heavy locking overhead          *
 *                 (fastest)                                                   *
 *              2. std::mutex:  Protects variables via software-level mutual   *
 *                 exclusion, blocking other threads until a lock is released  *
 *                 (slowest)                                                   *
 *              3. unprotected: Exposes the data race where threads clash and  *
 *                 overwrite each other's modifications simultaneously         *
 * Developer:   Jean-Milost Reymond                                            *
 *******************************************************************************/

// std
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>

//------------------------------------------------------------------------------
std::atomic<int> g_AtomicCounter(0);  // using std::atomic ensures updates are safe from data races
int              g_NormalCounter = 0;
int              g_MutexCounter  = 0;
std::mutex       g_CounterMutex;      // the lock that protects the variable
//------------------------------------------------------------------------------
void IncrementCounters()
{
    // test Atomic (maximum hardware concurrency)
    for (int i = 0; i < 10000; ++i)
        ++g_AtomicCounter;

    // test normal (no protection - will heavily corrupt now)
    for (int i = 0; i < 10000; ++i)
        ++g_NormalCounter;

    // test Mutex (software locking)
    for (int i = 0; i < 10000; ++i)
    {
        std::lock_guard<std::mutex> lock(g_CounterMutex);
        ++g_MutexCounter;
    }
}
//------------------------------------------------------------------------------
int main()
{
    std::vector<std::thread> threads;

    // launch 10 threads running at the same time
    for (int i = 0; i < 10; ++i)
        threads.emplace_back(IncrementCounters);

    // wait for all threads to finish
    for (auto& th : threads)
        th.join();

    // expected total for counters should be 100'000 (10 threads * 10'000)
    std::cout << "Atomic counter result: " << g_AtomicCounter << "\n";
    std::cout << "Mutex  counter result: " << g_MutexCounter  << "\n";
    std::cout << "Normal counter result: " << g_NormalCounter << "\n";

    return 0;
}
//------------------------------------------------------------------------------

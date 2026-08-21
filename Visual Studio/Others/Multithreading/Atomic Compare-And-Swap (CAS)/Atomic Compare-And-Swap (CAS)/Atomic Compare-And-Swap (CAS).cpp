/********************************************************************************
 * ==> Atomic Compare-And-Swap (CAS) Example -----------------------------------*
 ********************************************************************************
 * Description: This demo demonstrates Compare-And-Swap (CAS) operations using  *
 *              strong and weak atomic exchanges. CAS is the foundational       *
 *              building block of lock-free data structures, ensuring a value   *
 *              is only updated if it hasn't been modified by another thread.   *
 * Developer:   Jean-Milost Reymond                                             *
 ********************************************************************************/

// std
#include <iostream>
#include <atomic>

//------------------------------------------------------------------------------
void DemonstrateCompareExchangeStrong()
{
    std::cout << "--- compare_exchange_strong Demo ---\n";

    std::atomic<int> value(10);
    int expected = 10;
    int desired  = 20;

    // success Case: value matches expected (10). It safely updates to 20
    if (value.compare_exchange_strong(expected, desired))
        std::cout << "Success! Value matches expected. New value: " << value << "\n";

    // failure Case: Change expected to a wrong value (5) to force a failure
    expected = 5;

    // because value is 20 and expected is 5, the swap fails. CRITICAL: The hardware automatically overwrites expected with the
    // current value (20).
    if (!value.compare_exchange_strong(expected, desired))
        std::cout << "Failed! Expected value was automatically updated to current value: " << expected << "\n";

    std::cout << "\n";
}
//------------------------------------------------------------------------------
void DemonstrateCompareExchangeWeak()
{
    std::cout << "--- compare_exchange_weak (Lock-Free Loop) Demo ---\n";

    std::atomic<int> targetValue(10);

    int  currentExpected = targetValue.load();
    int  newDesired      = 0;
    bool success         = false;

    // multiply target_value by 3 safely across threads without using a mutex. NOTE compare_exchange_weak() can fail spuriously
    // (even if values match) due to CPU cache behaviors. Therefore, it MUST always be executed inside a loop until it succeeds
    while (!success)
    {
        // calculate the new state based on what we think the current value is
        newDesired = currentExpected * 3;

        // try the atomic update. If another thread modified targetValue in the background, or if a spurious CPU failure happens,
        // this returns false and updates currentExpected automatically
        success = targetValue.compare_exchange_weak(currentExpected, newDesired);

        if (!success)
            std::cout << "  Retrying loop... currentExpected' adjusted to: " << currentExpected << "\n";
    }

    std::cout << "Success! Final calculated value: " << targetValue << "\n";
}
//------------------------------------------------------------------------------
int main()
{
    DemonstrateCompareExchangeStrong();
    DemonstrateCompareExchangeWeak();
    return 0;
}
//------------------------------------------------------------------------------

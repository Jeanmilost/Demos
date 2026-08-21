/********************************************************************************
 * ==> Macro Expansion Issue Example -------------------------------------------*
 ********************************************************************************
 * Description: This demo illustrates a dangerous C++ trap caused by literal    *
 *              macro expansion. While a human reader assumes the addition      *
 *              inside CUBE(2 + 3) happens first, the preprocessor performs a   *
 *              blind text substitution before compilation. This injects the    *
 *              raw text into the expression, silently warping the operator     *
 *              precedence from 5 * 5 * 5 into 2 + 3 * 2 + 3 * 2 + 3.           *
 * Developer:   Jean-Milost Reymond                                             *
 ********************************************************************************/

// std
#include <iostream>

// The flawed macro (vulnerable to the macro expansion trap)
#define BUGGY_CUBE(x) (x * x * x)

// The fixed macro (using defensive parentheses)
#define FIXED_CUBE(x) ((x) * (x) * (x))

//------------------------------------------------------------------------------
int main()
{
    int input = 2 + 3; // evaluates to 5

    // 1. Buggy Macro Expansion Trap
    // Expected: 5 * 5 * 5 = 125
    // Actual textual expansion: (2 + 3 * 2 + 3 * 2 + 3) -> 2 + 6 + 6 + 3 = 17
    int buggyResult = BUGGY_CUBE(2 + 3);

    // 2. Fixed Macro Expansion
    // Actual textual expansion: ((2 + 3) * (2 + 3) * (2 + 3)) -> (5 * 5 * 5) = 125
    int fixedResult = FIXED_CUBE(2 + 3);

    // print the results to observe the trap
    std::cout << "Input expression: 2 + 3 (which equals 5)\n";
    std::cout << "---------------------------------------\n";
    std::cout << "BUGGY_CUBE(2 + 3) results in: " << buggyResult << " (Expected 125!)\n";
    std::cout << "FIXED_CUBE(2 + 3) results in: " << fixedResult << "\n";

    return 0;
}
//------------------------------------------------------------------------------

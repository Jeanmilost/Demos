/****************************************************************************
 * ==> C++ Exception Handling Mechanics & Pitfalls Demo --------------------*
 ****************************************************************************
 * Description: Demonstrates exception ordering constraints, stack          *
 *              unwinding lifecycles, rethrowing anonymous exceptions, and  *
 *              the runtime consequences of violating a noexcept contract   *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

// std
#include <iostream>
#include <stdexcept>

// NOTE: If you uncomment the define below, the code will FAIL TO COMPILE. The C++ standard mandates that all catch blocks must be
// reachable. A catch(...) placed first would swallow everything, turning specific handlers into dead code
//#define INCORRECT_CATCH_ORDER

//---------------------------------------------------------------------------
/**
* A simple tracker struct to visualize stack unwinding lifecycles
*/
struct Tracker
{
    std::string m_Name;
    bool        m_IsStatic;

    Tracker(std::string n, bool s = false) :
        m_Name(std::move(n)),
        m_IsStatic(s)
    {
        std::cout << " [Init] " << m_Name << " allocated on stack.\n";
    }

    ~Tracker()
    {
        if (m_IsStatic)
            std::cout << " [Exit] Static " << m_Name << " clean up at application shutdown.\n";
        else
            std::cout << " [Unwind] Local " << m_Name << " destroyed during stack unwinding.\n";
    }
};
//---------------------------------------------------------------------------
// MODULE 1: Stack Unwinding & Rethrowing Mechanics
//---------------------------------------------------------------------------
void ProcessDatabaseTransaction()
{
    Tracker localTx("Local_Transaction_Scope");
    static Tracker staticLog("Global_Application_Logger", true);

    std::cout << " -> Inside processDatabaseTransaction(), throwing runtime_error...\n";
    throw std::runtime_error("Database connection lost unexpectedy.");
}
//---------------------------------------------------------------------------
void IntermediateLayer()
{
    try
    {
        ProcessDatabaseTransaction();
    }
    catch (...)
    {
        std::cout << " -> Intermediate layer intercepted anonymous exception via catch-all (...).\n";
        std::cout << " -> Performing local emergency cleanup before passing the buck...\n";

        // rethrow the exact same exception up the call stack
        throw;
    }
}
//---------------------------------------------------------------------------
// MODULE 2: The noexcept Contract Violation
//---------------------------------------------------------------------------
// This function promises never to let an exception escape its boundaries
void RiskyNoexceptFunction() noexcept
{
    std::cout << " -> Inside riskyNoexceptFunction()...\n";

    // NOTE: This compiles! But because it escapes the function, it will
    // bypass all try-catch blocks and cause immediate runtime termination
    throw std::runtime_error("Critical failure within noexcept boundary.");
}
//---------------------------------------------------------------------------
// Main entry point
//---------------------------------------------------------------------------
int main()
{
    std::cout << "=== PHASE 1: CATCH ORDERING RULES ===\n";

    try
    {
        throw std::runtime_error("Test Error");
    }
#ifdef INCORRECT_CATCH_ORDER
    catch (...)
    {
        std::cout << "Catch-all error\n";
    }
#endif
    catch (const std::runtime_error& e)
    {
        std::cout << " Success: Caught specific runtime_error: " << e.what() << "\n";
    }
#ifndef INCORRECT_CATCH_ORDER
    catch (...)
    {
        std::cout << " Catch-all (Safe fallback placed at the absolute end).\n";
    }
#endif

    std::cout << "\n=== PHASE 2: STACK UNWINDING & RETHROW DEMO ===\n";

    try
    {
        IntermediateLayer();
    }
    catch (const std::exception& e)
    {
        std::cout << " Success: Main caught rethrown standard exception: " << e.what() << "\n";
    }

    std::cout << "\n=== PHASE 3: NOEXCEPT RUNTIME CONSEQUENCES ===\n";
    std::cout << "Press ENTER to execute the noexcept violation (This will intentionally crash the app)...";
    std::cin.get();

    try
    {
        RiskyNoexceptFunction();
    }
    catch (...)
    {
        // CRITICAL NOTE: This block will NEVER execute! When riskyNoexceptFunction() throws, std::terminate() is triggered instantly
        std::cout << "This line is unreachable. The application is already dead.\n";
    }

    return 0;
}
//---------------------------------------------------------------------------

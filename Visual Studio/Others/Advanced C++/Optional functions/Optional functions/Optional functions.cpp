/****************************************************************************
 * ==> Optional functions demo ---------------------------------------------*
 ****************************************************************************
 * Description: Demonstrates a std::optional usage and how the result is    *
 *              handled, instead of having to use workarounds or exceptions *
 *              to correctly handle unexpected results.                     *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************/

// std
#include <iostream>
#include <optional>
#include <string>

//---------------------------------------------------------------------------
/**
* Simulating a database search
*@param userId - user identifier to search
*@return The user matching the Id, null value if user was not found
*/
std::optional<std::string> findUserNickname(int userId)
{
    if (userId == 42)
        // automatically wraps into std::optional
        return "TheAnswer";

    // explicitly returns an empty optional (value not found)
    return std::nullopt;
}
//---------------------------------------------------------------------------
int main()
{
    int                        searchId = 99;
    std::optional<std::string> result   = findUserNickname(searchId);

    // method 1: Check using operator bool or .has_value()
    if (result)
        // access using pointer-like syntax (* or ->)
        std::cout << "User found: " << *result << "\n";
    else
        std::cout << "User ID " << searchId << " does not exist.\n";

    // method 2: Safe fallback using .value_or(). If result is empty, it uses the provided fallback string instead
    std::string displayName = result.value_or("Guest_User");

    // NOTE the single line above cleanly replaces the entire if/else check at the end
    std::cout << "Display name: " << displayName << "\n";

    // method 3: Direct extraction (throws std::bad_optional_access if empty)
    try
    {
        // NOTE VS2026 will generate a compilation warning on this line, see NOTE below
        std::cout << result.value() << "\n";
    }
    catch (const std::bad_optional_access& e)
    {
        std::cout << "Caught expected crash: " << e.what() << "\n";
    }

    // NOTE the above method 3 code will always throw an exception, and the VS2026 compiler knows that, it's the reason why the
    // code generates a warning during the compilation. Normally a such code should never be used in production (I kept it for the
    // example), instead the below code should be used
    if (result.has_value())
        // completely safe, zero warnings
        std::cout << result.value() << "\n";
    else
        std::cout << "No value to print.\n";

    return 0;
}
//---------------------------------------------------------------------------

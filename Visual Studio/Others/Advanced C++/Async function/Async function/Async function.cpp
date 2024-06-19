/****************************************************************************
 * ==> Async function ------------------------------------------------------*
 ****************************************************************************
 * Description : Demo showing how to run a function asynchronously          *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************/

// std
#include <string.h>
#include <iostream>
#include <sstream>
#include <future>

//---------------------------------------------------------------------------
int main()
{
    // declare a lambda function and call it asynchronously
    std::future<std::string> futureResult = std::async(
            [](const std::string& caption) -> std::string
            {
                std::ostringstream sstr;
                sstr << "This text: \"" << caption << "\" was built from an asynchronous function.\n";
                return sstr.str();
            },
            "Hello from the future");

    // wait for function to return the result
    const std::string result = futureResult.get();

    // log the returned result
    std::cout << result.c_str();
}
//---------------------------------------------------------------------------

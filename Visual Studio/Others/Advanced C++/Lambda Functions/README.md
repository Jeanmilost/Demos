# Lambda functions

Basically a lambda function is an unnamed function object capable of capturing variables in scope.

It may take the following form (among others, but most commonly used in our code):

```cpp
[Captures](Params){Body}
```

Where:

- **Captures** is a comma-separated list of zero or more parameters accessible locally, e.g. the this pointer, a local variable, a variable passed as a parameter of a calling function, ...
- **Params** are the function parameters
- **Body** is the content of the function

## Demo code

Below is an example of code showing how lambda functions may be declared and called in different contexts:

```cpp
#include <string>
#include <iostream>
#include <sstream>

/**
* Class to call a lambda function to sum numbers
*/
class CallingLambda
{
    public:
        CallingLambda(int a, int b) :
            m_A(a),
            m_B(b)
        {}

        /**
        * Resolves the lambda function and returns the result
        *@return string containing the lambda function result
        */
        auto Resolve()
        {
            // define a lambda function, allowing access to the class members, and which returns a string with a calculated sum
            auto lambda = [this](const std::string& text)
            {
                std::ostringstream sstr;
                sstr << text << " - a - " << m_A << " - b - " << m_B << " - sum - " << (m_A + m_B) << "\n";
                return sstr.str();
            };

            return lambda("Lambda function 4 - CALLED");
        }

        /**
        * Resolves the lambda function, adds an external number to the result, and returns it
        *@param number - external number to add to the sum which the lambda function will resolve
        *@return string containing the lambda function result
        */
        auto ResolveAndAdd(int number)
        {
            // define a lambda function, allowing access to the class members and passing a parameter in captures, and which returns
            // a string with a calculated sum
            auto lambda = [this, number](const std::string& text)
            {
                std::ostringstream sstr;
                sstr << text << " - a - " << m_A << " - b - " << m_B << " - number - " << number << " - sum - " << (m_A + m_B + number) << "\n";
                return sstr.str();
            };

            return lambda("Lambda function 5 - CALLED");
        }

    private:
        int m_A = 0;
        int m_B = 0;
};

/**
* Main function
*/
int main()
{
    // define a simple lambda function, without parameters, which returns a string
    auto lambda1 = []()
    {
        return "Lambda function 1 - CALLED\n";
    };

    // call it and log the result
    std::cout << lambda1();

    const int a = 3;
    const int b = 4;

    // define a lambda function, with parameters passed as captures, and returns a string with a calculated sum
    auto lambda2 = [a, b]()
    {
        std::ostringstream sstr;
        sstr << "Lambda function 2 - CALLED - a - " << a << " - b - " << b << " - sum - " << (a + b) << "\n";
        return sstr.str();
    };

    // call it and log the result
    std::cout << lambda2();

    // define a lambda function, with parameters passed as arguments, and returns a string with a calculated sum
    auto lambda3 = [](int a, int b)
    {
        std::ostringstream sstr;
        sstr << "Lambda function 3 - CALLED - a - " << a << " - b - " << b << " - sum - " << (a + b) << "\n";
        return sstr.str();
    };

    // call it and log the result
    std::cout << lambda3(a, b);

    // create a class instance which will call the lambda internally
    CallingLambda lambda4(a, b);

    // call it and log the result
    std::cout << lambda4.Resolve();
    std::cout << lambda4.ResolveAndAdd(8);
}

```

## References

https://learn.microsoft.com/en-us/cpp/cpp/lambda-expressions-in-cpp?view=msvc-170

https://learn.microsoft.com/en-us/cpp/cpp/examples-of-lambda-expressions?view=msvc-170

https://en.cppreference.com/w/cpp/language/lambda

https://www.geeksforgeeks.org/lambda-expression-in-c/
# Callback functions

A callback is any executable code that is passed as an argument to another code, which is expected to call back (execute) the argument at a given time. In simple language, if a reference of a function is passed to another function as an argument to call it, then it will be called a Callback function.

## Difference between a *function* and a *method*

There is an important distinction to consider between a *function* and a *method*, which is required to well understand the callbacks:

- A *function* is a piece of code that is called by name. It can be passed data to operate on (by the parameters) and can optionally return data (the return value). All data that is passed to a *function* is explicitly passed
- A *method* is a piece of code that is called by name that is associated with an object. In most respects, it is identical to a *function*, except for two key differences:
    - A *method* is implicitly passed data to operate on by the object on which it was called
    - A *method* is able to operate on data that is contained within the class (remembering that an object is an instance of a class - the class is the definition, the object is an instance of that data)

This is a simplified explanation, ignoring issues of scope, etc...

## C callbacks

The C callbacks are simply a *function* passed as a parameter to another *function*. It is declared as follow:

```cpp
typedef *result_type* (**callback_name*)(*arguments*);
```

Where:

- *result_type* is the resulting type the callback will return. It may be void
- *callback_name* is the name of the callback
- *arguments* is the argument list required to call the callback. They may be void

The C callbacks are very simple to implement (see the *SimpleCallback* class in the code below), and are widely supported (i.e. even very old versions of the C and C++ support them), but they have a strong limitation, they don’t support *methods*. You can either call a *function,* or at best a static member *function* declared in a class, but not a *method*.

## Embarcadero RAD Studio callbacks

To workaround the C callbacks limitation, the Embarcadero RAD Studio compilers introduced a new keyword, named __closure. On these compilers a callback may be defined as follow:

```cpp
typedef result_type (__closure **callback_name*)(*arguments*);
```

Where:

- *result_type* is the resulting type the callback will return. It may be void
- *callback_name* is the name of the callback
- *arguments* is the argument list required to call the callback. They may be void

Contrary to the C callbacks, these callbacks are supporting *methods*, but are limited to the Embarcadero RAD Studio compilers.

## STD callbacks

Since C++ 11, the STD includes natively the previously declared callbacks in the Boost library. In every compilers which support this version of C++ or higher, a callback may be defined as follow:

```cpp
typedef std::function<result_type(*arguments*)> *callback_name*;
```

Where:

- *result_type* is the resulting type the callback will return. It may be void
- *arguments* is the argument list required to call the callback. They may be void
- *callback_name* is the name of the callback

Similar to a Boost callback, a STD callback is a kind of *std::function*. This means that the callback *method* need to be bind, in the following manner:

```cpp
std::bind(&method_to_call, &method_owner, param_1, …)
```

Where:

- *method_to_call* is the address of the *method* to call
- *method_owner* is the owner of the *method*, it may be this
- *param_1* is the first parameter of the arguments list. The bind may contain as parameters as the ones declared in the argument list

Of course the limitation is that the compiler needs to support C++ 11 or higher in order to use such callbacks.

## Boost callbacks

The Boost library also introduced a solution to declare callbacks which support the call to a *method*. They may be defined as follow:

```cpp
typedef boost::function<result_type(*arguments*)> *callback_name*;
```

Where:

- *result_type* is the resulting type the callback will return. It may be void
- *arguments* is the argument list required to call the callback. They may be void
- *callback_name* is the name of the callback

As you can see, this time the callback is a kind of *boost::function*. This means that the callback *method* need to be bind, in the following manner:

```cpp
boost::bind(&method_to_call, &method_owner, param_1, …)
```

Where:

- *method_to_call* is the address of the *method* to call
- *method_owner* is the owner of the *method*, it may be this
- *param_1* is the first parameter of the arguments list. The bind may contain as parameters as the ones declared in the argument list

These callbacks may be used everywhere the Boost library is included, but of course they depend of it.

## Demo code

```cpp
// std
#include <iostream>
#include <sstream>

// libraries
#include <boost/bind.hpp>
#include <boost/function.hpp>

/**
* C callback
*/
class CCallback
{
    public:
        /**
        * Callback function definition
        */
        typedef std::string (*ITfOnCallback)(const std::string& caption);

        CCallback();
        virtual ~CCallback();

        /**
        * Calls the callback
        */
        void Call();

        /**
        * Sets the callback to call
        *@param callback - callback handle
        */
        void Set_Callback(const ITfOnCallback& callback);

        /**
        * Callback method to call
        *@param caption - caption
        *@return string to write to output stream
        */
        static std::string OnCallback(const std::string& caption);

    private:
        ITfOnCallback m_Callback = nullptr;
};

//---------------------------------------------------------------------------
CCallback::CCallback()
{}
//---------------------------------------------------------------------------
CCallback::~CCallback()
{}
//---------------------------------------------------------------------------
void CCallback::Call()
{
    if (m_Callback)
    {
        std::cout << m_Callback("CCallback").c_str();
        return;
    }

    std::cout << "CCallback - ERROR - callback not defined";
}
//---------------------------------------------------------------------------
void CCallback::Set_Callback(const ITfOnCallback& callback)
{
    m_Callback = callback;
}
//---------------------------------------------------------------------------
std::string CCallback::OnCallback(const std::string& caption)
{
    std::ostringstream sstr;
    sstr << "Called from: " << caption << "\n";
    return sstr.str();
}
//---------------------------------------------------------------------------

/**
* Std callback (since c++ 11)
*/
class StdCallback
{
    public:
        /**
        * Callback function definition
        */
        typedef std::function<std::string(const std::string& caption)> ITfOnCallback;

        StdCallback();
        virtual ~StdCallback();

        /**
        * Calls the callback
        */
        void Call();

        /**
        * Sets the callback to call
        *@param callback - callback handle
        *@note The method to call should be bind in the following manner:
        *      Set_Callback(std::bind(&method_to_call, &method_owner, param_1, ...))
        */
        void Set_Callback(const ITfOnCallback& callback);

        /**
        * Callback method to call
        *@param caption - caption
        *@return string to write to output stream
        */
        std::string OnCallback(const std::string& caption);

    private:
        ITfOnCallback m_Callback;
};

//---------------------------------------------------------------------------
StdCallback::StdCallback()
{}
//---------------------------------------------------------------------------
StdCallback::~StdCallback()
{}
//---------------------------------------------------------------------------
void StdCallback::Call()
{
    if (m_Callback)
    {
        std::cout << m_Callback("StdCallback").c_str();
        return;
    }

    std::cout << "StdCallback - ERROR - callback not defined";
}
//---------------------------------------------------------------------------
void StdCallback::Set_Callback(const ITfOnCallback& callback)
{
    m_Callback = callback;
}
//---------------------------------------------------------------------------
std::string StdCallback::OnCallback(const std::string& caption)
{
    std::ostringstream sstr;
    sstr << "Called from: " << caption << "\n";
    return sstr.str();
}
//---------------------------------------------------------------------------

/**
* Boost callback
*/
class BoostCallback
{
    public:
        /**
        * Callback function definition
        */
        typedef boost::function<std::string(const std::string& caption)> ITfOnCallback;

        BoostCallback();
        virtual ~BoostCallback();

        /**
        * Calls the callback
        */
        void Call();

        /**
        * Sets the callback to call
        *@param callback - callback handle
        *@note The method to call should be bind in the following manner:
        *      Set_Callback(boost::bind(&method_to_call, &method_owner, param_1, ...))
        */
        void Set_Callback(const ITfOnCallback& callback);

        /**
        * Callback method to call
        *@param caption - caption
        *@return string to write to output stream
        */
        std::string OnCallback(const std::string& caption);

    private:
        ITfOnCallback m_Callback;
};

//---------------------------------------------------------------------------
BoostCallback::BoostCallback()
{}
//---------------------------------------------------------------------------
BoostCallback::~BoostCallback()
{}
//---------------------------------------------------------------------------
void BoostCallback::Call()
{
    if (m_Callback)
    {
        std::cout << m_Callback("BoostCallback").c_str();
        return;
    }

    std::cout << "BoostCallback - ERROR - callback not defined";
}
//---------------------------------------------------------------------------
void BoostCallback::Set_Callback(const ITfOnCallback& callback)
{
    m_Callback = callback;
}
//---------------------------------------------------------------------------
std::string BoostCallback::OnCallback(const std::string& caption)
{
    std::ostringstream sstr;
    sstr << "Called from: " << caption << "\n";
    return sstr.str();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
int main()
{
    // c callback. The callback function can be a static member of a class or a standalone function, but not a method
    CCallback cCallback;
    cCallback.Set_Callback(CCallback::OnCallback);
    cCallback.Call();

    // std callback, supports methods, and similar to boost callbacks
    StdCallback stdCallback;
    stdCallback.Set_Callback(std::bind(&StdCallback::OnCallback, &stdCallback, std::placeholders::_1));
    stdCallback.Call();

    // boost callback, supports methods, and similar to std callbacks
    BoostCallback boostCallback;
    boostCallback.Set_Callback(boost::bind(&BoostCallback::OnCallback, &boostCallback, boost::placeholders::_1));
    boostCallback.Call();
}
//---------------------------------------------------------------------------
```

## References

https://stackoverflow.com/questions/155609/whats-the-difference-between-a-method-and-a-function

https://blog.stratifylabs.dev/device/2022-12-01-Callback-and-Lambdas-in-embedded-cpp/
/****************************************************************************
 * ==> Callbacks -----------------------------------------------------------*
 ****************************************************************************
 * Description : Demo showing how most basic callbacks may be called in     *
 *               their different contexts                                   *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************/

// std
#include <iostream>
#include <sstream>
#include <functional>

// libraries
#include <boost/bind.hpp>
#include <boost/function.hpp>

// NOTE about the difference between a function and a method:
// (from https://stackoverflow.com/questions/155609/whats-the-difference-between-a-method-and-a-function)
// - A function is a piece of code that is called by name.It can be passed data to operate on(by the parameters) and can optionally
//   return data(the return value).All data that is passed to a function is explicitly passed
// - A method is a piece of code that is called by name that is associated with an object.In most respects, it is identical to a function
//   except for two key differences:
//   -> A method is implicitly passed data to operate on by the object on which it was called
//   -> A method is able to operate on data that is contained within the class (remembering that an object is an instance of a class - the
//      class is the definition, the object is an instance of that data)
// This is a simplified explanation, ignoring issues of scope, etc...

/**
* Simple C callback
*/
class SimpleCallback
{
    public:
        /**
        * Callback function definition
        */
        typedef std::string (*ITfOnCallback)(const std::string& caption);

        SimpleCallback();
        virtual ~SimpleCallback();

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
// SimpleCallback
//---------------------------------------------------------------------------
SimpleCallback::SimpleCallback()
{}
//---------------------------------------------------------------------------
SimpleCallback::~SimpleCallback()
{}
//---------------------------------------------------------------------------
void SimpleCallback::Call()
{
    if (m_Callback)
    {
        std::cout << m_Callback("SimpleCallback").c_str();
        return;
    }

    std::cout << "SimpleCallback - ERROR - callback not defined";
}
//---------------------------------------------------------------------------
void SimpleCallback::Set_Callback(const ITfOnCallback& callback)
{
    m_Callback = callback;
}
//---------------------------------------------------------------------------
std::string SimpleCallback::OnCallback(const std::string& caption)
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
// StdCallback
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
// BoostCallback
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
    SimpleCallback simpleCallback;
    simpleCallback.Set_Callback(SimpleCallback::OnCallback);
    simpleCallback.Call();

    StdCallback stdCallback;
    stdCallback.Set_Callback(std::bind(&StdCallback::OnCallback, &stdCallback, std::placeholders::_1));
    stdCallback.Call();

    BoostCallback boostCallback;
    boostCallback.Set_Callback(boost::bind(&BoostCallback::OnCallback, &boostCallback, boost::placeholders::_1));
    boostCallback.Call();
}
//---------------------------------------------------------------------------

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
#include <csignal>

// libraries
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/optional/optional_io.hpp>

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
// CCallback
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

/**
* Std signal, since c++ 17
*/
class StdSignal
{
    public:
        /**
        * Signal function definition, need to be of type void(int)
        */
        typedef void (*ITfOnSignal)(int signal);

        StdSignal();
        virtual ~StdSignal();

        /**
        * Emits the signal
        */
        void Emit();

        /**
        * Sets the signal to emit
        *@param channel - signal channel, may be a macro like SIGINT or an implementation-defined value
        *@param signal - signal handle
        */
        void Set_Signal(int channel, const ITfOnSignal& signal);

        /**
        * Signal method to call
        *@param channel - signal channel
        */
        static void OnSignal(int channel);

    private:
        int         m_Channel = 0;
        ITfOnSignal m_Signal  = nullptr;
};

//---------------------------------------------------------------------------
// StdSignal
//---------------------------------------------------------------------------
StdSignal::StdSignal()
{}
//---------------------------------------------------------------------------
StdSignal::~StdSignal()
{}
//---------------------------------------------------------------------------
void StdSignal::Emit()
{
    if (m_Signal)
    {
        std::raise(m_Channel);
        return;
    }

    std::cout << "StdSignal - ERROR - callback not defined";
}
//---------------------------------------------------------------------------
void StdSignal::Set_Signal(int channel, const ITfOnSignal& signal)
{
    m_Signal  = signal;
    m_Channel = channel;

    std::signal(m_Channel, m_Signal);
}
//---------------------------------------------------------------------------
void StdSignal::OnSignal(int channel)
{
    std::ostringstream sstr;
    sstr << "Called from: StdSignal, channel = " << channel << "\n";
    std::cout << sstr.str();
}
//---------------------------------------------------------------------------

/**
* Boost signal
*/
class BoostSignal
{
    public:
        /**
        * Signal function definition
        */
        typedef boost::signals2::signal<std::string(const std::string& caption)> ITfOnSignal;

        BoostSignal();
        virtual ~BoostSignal();

        /**
        * Emits the signal
        */
        void Emit();

        /**
        * Sets the signal to emit
        *@param signal - signal handle
        *@note The signal to emit should be bind in the following manner:
        *      Set_Signal(boost::bind(&method_to_call, &method_owner, param_1, ...))
        */
        void Set_Signal(const ITfOnSignal::slot_type& signal);

        /**
        * Signal method to call
        *@param caption - caption
        *@return string to write to output stream
        */
        std::string OnSignal(const std::string& caption);

    private:
        ITfOnSignal m_Signal;
};

//---------------------------------------------------------------------------
// BoostSignal
//---------------------------------------------------------------------------
BoostSignal::BoostSignal() :
    m_Signal()
{}
//---------------------------------------------------------------------------
BoostSignal::~BoostSignal()
{}
//---------------------------------------------------------------------------
void BoostSignal::Emit()
{
    if (!m_Signal.empty())
    {
        std::cout << m_Signal("BoostSignal").value().c_str();
        return;
    }

    std::cout << "BoostSignal - ERROR - callback not defined";
}
//---------------------------------------------------------------------------
void BoostSignal::Set_Signal(const ITfOnSignal::slot_type& signal)
{
    m_Signal.connect(signal);
}
//---------------------------------------------------------------------------
std::string BoostSignal::OnSignal(const std::string& caption)
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

    // std signal. The callback function can be a static member of a class or a standalone function, but not a method
    StdSignal stdSignal;
    stdSignal.Set_Signal(SIGINT, StdSignal::OnSignal);
    stdSignal.Emit();

    // boost signal, supports methods
    BoostSignal boostSignal;
    boostSignal.Set_Signal(boost::bind(&BoostSignal::OnSignal, &boostSignal, boost::placeholders::_1));
    boostSignal.Emit();
}
//---------------------------------------------------------------------------

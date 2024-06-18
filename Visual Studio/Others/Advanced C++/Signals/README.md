# Signals

Signals are close to callback functions, but with some advanced functionalities, which place them between the callback and the observer. They support, among others:

- Multiple callback functions on the same slot
- Thread safety

## STD Signals

STD signals are the interrupts that force an OS to stop its ongoing task and attend the task for which the interrupt has been sent. These interrupts can pause service in any program of an OS. Similarly, C++ also offers various signals which it can catch and process in a program. They may be defined as follow:

```cpp
typedef void (**signal_name*)(int *slot*);
```

Where:

- *signal_name* is the name of the signal
- *slot* is the slot (or channel) on which the signal was emitted

You can connect the signal in the following manner:

```cpp
std::signal(*slot*, *signal*);
```

Where:

- *slot* is the signal slot on which the signal will be emitted
- *signal* is the signal function to call

Once connected, you can emit the signal with the following code

```cpp
std::raise(*slot*);
```

Where:

- *slot* is the signal slot on which the signal was previously connected

**NOTE** the signal slot can be a pre-defined standard slot like SIGINT or SIGTERM, or an user-defined signal slot. See https://www.geeksforgeeks.org/signal-handling-in-cpp/ for further information.

As you can see, STD signals are is more rigid than other signals or callbacks, and cannot return value or pass additional arguments. This is because their purpose is different than for other signals and callbacks, as described above. It is not very used in our applications.

## Boost signals

The Boost signals library is an implementation of a managed signals and slots system. Signals represent callbacks with multiple targets, and are also called publishers or events in similar systems. Signals are connected to some set of slots, which are callback receivers (also called event targets or subscribers), which are called when the signal is "emitted."

Signals and slots are managed, in that signals and slots (or, more properly, objects that occur as part of the slots) can track connections and are capable of automatically disconnecting signal/slot connections when either is destroyed. This enables the user to make signal/slot connections without expending a great effort to manage the lifetimes of those connections with regard to the lifetimes of all objects involved.

Boost signals are also thread safe.

They may be defined as follow:

```cpp
typedef boost::signals2::signal<*result_type*(*arguments*)> *signal_name*;
```

Where:

- *result_type* is the resulting type the callback will return. It may be void
- *arguments* is the argument list required to call the callback. They may be void
- *signal_name* is the name of the signal

The signal can be connected in the following manner:

```cpp
*signal*.connect(*slot*);
```

Where:

- *signal* is the declared signal (from *signal_name* above)
- *slot* is the signal slot to connect

The signal is a kind of *boost::signals2::signal*. This means that the slot may be declared in the following manner:

```cpp
boost::bind(&method_to_call, &method_owner, param_1, …)
```

Where:

- *method_to_call* is the address of the *method* to call
- *method_owner* is the owner of the *method*, it may be this
- *param_1* is the first parameter of the arguments list. The bind may contain as parameters as the ones declared in the argument list

Once connected, the signal can be emitted with the following code:

```cpp
if (!signal.empty())
    signal(arguments);
```

These signals may be used everywhere the Boost library is included, but of course they depend of it.

## Sciter and Qt signals

*Sciter* and *Qt* also use signals to connect to their interfaces. Please refer to the documentation provided by these libraries to know how to use them.

## Demo code

```cpp
 // std
#include <iostream>
#include <sstream>
#include <csignal>

// libraries
#include <boost/bind.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>

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
        ITfOnSignal m_Signal = nullptr;
};

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

    std::cout << "StdSignal - ERROR - signal not defined";
}
//---------------------------------------------------------------------------
void StdSignal::Set_Signal(int channel, const ITfOnSignal& signal)
{
    m_Signal = signal;
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

    std::cout << "BoostSignal - ERROR - signal not defined";
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

/**
* Boost multiple signals
*/
class BoostMultiSignal
{
    public:
        /**
        * Signal function definition
        */
        typedef boost::signals2::signal<void()> ITfOnSignal;

        BoostMultiSignal();
        virtual ~BoostMultiSignal();

        /**
        * Emits the signal
        */
        void Emit();

        /**
        * Adds a signal to emit
        *@param signal - signal handle
        *@note The signal to emit should be bind in the following manner:
        *      Set_Signal(boost::bind(&method_to_call, &method_owner, param_1, ...))
        */
        void Add_Signal(const ITfOnSignal::slot_type& signal);

        /**
        * First signal method to call
        */
        void OnSignalLine1();

        /**
        * Second signal method to call
        */
        void OnSignalLine2();

    private:
        ITfOnSignal m_Signal;
};

//---------------------------------------------------------------------------
BoostMultiSignal::BoostMultiSignal() :
    m_Signal()
{}
//---------------------------------------------------------------------------
BoostMultiSignal::~BoostMultiSignal()
{}
//---------------------------------------------------------------------------
void BoostMultiSignal::Emit()
{
    if (!m_Signal.empty())
    {
        m_Signal();
        return;
    }

    std::cout << "BoostMultiSignal - ERROR - signal not defined";
}
//---------------------------------------------------------------------------
void BoostMultiSignal::Add_Signal(const ITfOnSignal::slot_type& signal)
{
    m_Signal.connect(signal);
}
//---------------------------------------------------------------------------
void BoostMultiSignal::OnSignalLine1()
{
    std::cout << "Called from: BoostMultiSignal - First signal\n";
}
//---------------------------------------------------------------------------
void BoostMultiSignal::OnSignalLine2()
{
    std::cout << "Called from: BoostMultiSignal - Second signal\n";
}
//---------------------------------------------------------------------------

/**
* I/O thread
*/
class Thread
{
    public:
        Thread();
        ~Thread();

        /**
        * Starts the background thread
        */
        bool Start();

        /**
        * Runs the thread (main loop)
        */
        void Run();

        /**
        * Closes the thread
        */
        void Close();

        /**
        * Post the message on the thread
        *@param msg - message to post
        */
        void Post(std::string msg);

        /**
        * Threaded signal method to call
        *@param caption - caption
        *@return string to write to output stream
        */
        void OnSignal(const std::string& caption);

    private:
        bool                                                                     m_Started = false;
        boost::asio::io_service                                                  m_Service;
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> m_Worker;
        boost::thread                                                            m_Thread;
};

//---------------------------------------------------------------------------
Thread::Thread() :
    m_Worker(boost::asio::make_work_guard(m_Service))
{}
//---------------------------------------------------------------------------
Thread::~Thread()
{}
//---------------------------------------------------------------------------
bool Thread::Start()
{
    if (m_Started)
        return true;

    m_Started = true;

    // start reader thread
    m_Thread = boost::thread(boost::bind(&Thread::Run, this));

    return m_Started;
}
//---------------------------------------------------------------------------
void Thread::Run()
{
    m_Service.run();
}
//---------------------------------------------------------------------------
void Thread::Close()
{
    m_Worker.reset();

    if (m_Thread.joinable())
        m_Thread.join();

    m_Started = false;
}
//---------------------------------------------------------------------------
void Thread::Post(std::string msg)
{
    m_Service.post(boost::bind(&Thread::OnSignal, this, msg));
}
//---------------------------------------------------------------------------
void Thread::OnSignal(const std::string& caption)
{
    std::cout << "Called from: " << caption.c_str() << "\n";
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
int main()
{
    // std signal. The callback function can be a static member of a class or a standalone function, but not a method
    StdSignal stdSignal;
    stdSignal.Set_Signal(SIGINT, StdSignal::OnSignal);
    stdSignal.Emit();

    // boost signal, supports methods
    BoostSignal boostSignal;
    boostSignal.Set_Signal(boost::bind(&BoostSignal::OnSignal, &boostSignal, boost::placeholders::_1));
    boostSignal.Emit();

    // boost multiple signals, supports methods
    BoostMultiSignal boostMultiSignal;
    boostMultiSignal.Add_Signal(boost::bind(&BoostMultiSignal::OnSignalLine1, &boostMultiSignal));
    boostMultiSignal.Add_Signal(boost::bind(&BoostMultiSignal::OnSignalLine2, &boostMultiSignal));
    boostMultiSignal.Emit();

    Thread thread;

    // start the thread
    thread.Start();

    // connect the signal to call through the thread
    boost::signals2::signal<void(std::string)> threadedSignal;
    threadedSignal.connect(boost::bind(&Thread::Post, boost::ref(thread), _1));

    // send the signal
    threadedSignal("BoostThreadedSignal");

    // wait and quit
    boost::this_thread::sleep(boost::chrono::seconds(2));
    thread.Close();
}
//---------------------------------------------------------------------------
```

## References

https://www.geeksforgeeks.org/signal-handling-in-cpp/

https://www.tutorialspoint.com/cplusplus/cpp_signal_handling.htm

https://www.boost.org/doc/libs/1_60_0/doc/html/signals2.html

[https://www.boost.org/doc/libs/1_82_0/doc/html/signals2/tutorial.html](https://www.boost.org/doc/libs/1_82_0/doc/html/signals2/tutorial.html#id-1.3.35.4.4)

https://stackoverflow.com/questions/34237190/why-are-signals-and-slots-better-than-plain-old-callbacks

https://stackoverflow.com/questions/5050588/how-in-boost-send-a-signal-in-a-thread-and-have-the-corresponding-slot-executed
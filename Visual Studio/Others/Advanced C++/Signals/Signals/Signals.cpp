/****************************************************************************
 * ==> Signals -------------------------------------------------------------*
 ****************************************************************************
 * Description : Demo showing how signals may be called in their different  *
 *               contexts                                                   *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************/

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
// BoostMultiSignal
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
// Main entry point
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

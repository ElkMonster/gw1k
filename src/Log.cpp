#include "Log.h"

#include <iostream>

namespace gw1k
{


const std::string Log::sLogWarning_("gw1k WARNING");
std::ostringstream Log::buffer_;


std::ostringstream&
Log::os()
{
    return buffer_;
}


void
Log::warning(const char* who, const char* msg, bool bEndl)
{
    print(LOG_WARNING, who, msg, bEndl);
}


void
Log::warning(const char* who, const std::string& msg, bool bEndl)
{
    print(LOG_WARNING, who, msg.c_str(), bEndl);
}


void
Log::warning(const char* who, const std::ostringstream& msg, bool bEndl)
{
    print(LOG_WARNING, who, msg.str().c_str(), bEndl);
    if (&msg == &buffer_)
    {
        buffer_.clear();
    }
}


void
Log::print(MsgType msgType, const char* who, const char* msg, bool bEndl)
{
    const std::string* preamble;
    switch (msgType)
    {
    case LOG_WARNING:
        preamble = &sLogWarning_;
    }
    std::cout << *preamble << " * " << who << ": " << msg;
    if (bEndl)
    {
        std::cout << std::endl;
    }
}


} // namespace gw1k

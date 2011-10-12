#include "Log.h"

#include <iostream>

namespace gw1k
{


const std::string Log::sLogWarning_("gw1k WARNING");
const std::string Log::sLogError_("gw1k ERROR");
const std::string Log::sLogInfo_("gw1k INFO");

std::ostringstream Log::buffer_;


std::ostringstream&
Log::os()
{
    return buffer_;
}


#define ADD_LOG_HANDLERS(HDLR_FN_NAME, HDLR_TYPE_NAME)                  \
void                                                                    \
Log::HDLR_FN_NAME(const char* who, const char* msg, bool bEndl)         \
{                                                                       \
    print(HDLR_TYPE_NAME, who, msg, bEndl);                             \
}                                                                       \
                                                                        \
                                                                        \
void                                                                    \
Log::HDLR_FN_NAME(const char* who, const std::ostream& msg, bool bEndl) \
{                                                                       \
    print(HDLR_TYPE_NAME, who, msg, bEndl);                             \
}                                                                       \

ADD_LOG_HANDLERS(warning, LOG_WARNING)
ADD_LOG_HANDLERS(error, LOG_ERROR)
ADD_LOG_HANDLERS(info, LOG_INFO)


void
Log::print(MsgType msgType, const char* who, const char* msg, bool bEndl)
{
    const std::string* preamble;
    switch (msgType)
    {
    case LOG_WARNING:
        preamble = &sLogWarning_;
        break;
    case LOG_ERROR:
        preamble = &sLogError_;
        break;
    case LOG_INFO:
        preamble = &sLogInfo_;
        break;
    }
    std::cout << *preamble << " * " << who << ": " << msg;
    if (bEndl)
    {
        std::cout << std::endl;
    }
}


void
Log::print(MsgType msgType, const char* who, const std::ostream& msg, bool bEndl)
{
    if (&msg == &buffer_)
    {
        print(msgType, who, buffer_.str().c_str(), bEndl);
        buffer_.str("");
    }
    else
    {
        std::stringstream ss;
        ss << msg;
        print(msgType, who, ss.str().c_str(), bEndl);
    }
}

} // namespace gw1k

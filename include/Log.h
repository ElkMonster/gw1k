#ifndef GW1K_LOG_H_
#define GW1K_LOG_H_

#include <string>
#include <sstream>


namespace gw1k
{


class Log
{

public:

    enum MsgType { LOG_WARNING, LOG_ERROR, LOG_INFO };

public:

    static std::ostringstream& os();

    static void warning(const char* who, const char* msg, bool bEndl = true);

    static void warning(const char* who, const std::ostream& msg, bool bEndl = true);

    static void error(const char* who, const char* msg, bool bEndl = true);

    static void error(const char* who, const std::ostream& msg, bool bEndl = true);

    static void info(const char* who, const char* msg, bool bEndl = true);

    static void info(const char* who, const std::ostream& msg, bool bEndl = true);

private:

    static void print(MsgType msgType, const char* who, const char* msg, bool bEndl);

    static void print(MsgType msgType, const char* who, const std::ostream& msg, bool bEndl);

    static std::ostringstream buffer_;

    static const std::string sLogWarning_;

    static const std::string sLogError_;

    static const std::string sLogInfo_;

};


} // namespace gw1k


#endif // GW1K_LOG_H_

#ifndef GW1K_EXCEPTION_H_
#define GW1K_EXCEPTION_H_

#include <exception>
#include <string>

namespace gw1k
{


class Exception : public std::exception
{

public:

    Exception(const std::string& msg)
    :   std::exception(),
        msg_(msg)
    {}

    virtual ~Exception() throw() {}

    virtual const char* what() const throw()
    {
        return msg_.c_str();
    }

private:

    std::string msg_;

};


} // namespace gw1k

#endif // GW1K_EXCEPTION_H_

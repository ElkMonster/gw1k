#include "Timer.h"

#include "GuiObject.h"
#include <cmath>

namespace gw1k
{


Timer::Timer(double seconds, GuiObject* target, int userdata)
:   target(target),
    userdata(userdata)
{
    newTimeout(seconds);
}


Timer::~Timer()
{

}


bool
Timer::expired() const
{
    timeval now;
    gettimeofday(&now, 0);
    return expired(now);
}


bool
Timer::expired(const timeval& now) const
{
    return *this < now;
}


void
Timer::newTimeout(double seconds)
{
    timeval now;
    gettimeofday(&now, 0);

    tvEnd_.tv_sec = now.tv_sec + std::floor(seconds);
    double intpart;
    tvEnd_.tv_usec = now.tv_usec + std::modf(seconds, &intpart) * 1e6;
    if (tvEnd_.tv_usec >= 10e6)
    {
        tvEnd_.tv_usec -= 10e6;
        tvEnd_.tv_sec += 1;
    }
}


const timeval&
Timer::getExpirationTime() const
{
    return tvEnd_;
}


bool
Timer::operator<(const timeval& time) const
{
    if (tvEnd_.tv_sec < time.tv_sec)
    {
        return true;
    }
    else if (tvEnd_.tv_sec < time.tv_sec)
    {
        return false;
    }
    else // tvEnd_.tv_sec == time.tv_sec
    {
        return tvEnd_.tv_usec < time.tv_usec;
    }
}


bool
Timer::operator>(const timeval& time) const
{
    if (tvEnd_.tv_sec > time.tv_sec)
    {
        return true;
    }
    else if (tvEnd_.tv_sec > time.tv_sec)
    {
        return false;
    }
    else // tvEnd_.tv_sec == time.tv_sec
    {
        return tvEnd_.tv_usec > time.tv_usec;
    }
}


bool
Timer::operator==(const timeval& time) const
{
    return (tvEnd_.tv_sec == time.tv_sec) && (tvEnd_.tv_usec == time.tv_usec);
}


bool
Timer::operator>=(const timeval& time) const
{
    return (*this > time) || (*this == time);
}


} // namespace gw1k

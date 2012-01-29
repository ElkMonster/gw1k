#ifndef GW1K_TIMER_H_
#define GW1K_TIMER_H_

#include "listeners/TimerListener.h"

#include <sys/time.h>

namespace gw1k
{


class Timer
{

public:

    Timer(double seconds, TimerListener* target, int userdata);

    ~Timer();

public:

    bool expired() const;

    bool expired(const timeval& now) const;

    void newTimeout(double seconds);

    const timeval& getExpirationTime() const;

    bool operator<(const timeval& time) const;

    bool operator>(const timeval& time) const;

    bool operator==(const timeval& time) const;

    bool operator>=(const timeval& time) const;

public:

    TimerListener* const target;

    const int userdata;

private:

    timeval tvEnd_;

};


} // namespace gw1k

#endif // GW1K_TIMER_H_

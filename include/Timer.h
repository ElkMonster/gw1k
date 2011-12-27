#ifndef GW1K_TIMER_H_
#define GW1K_TIMER_H_

#include <sys/time.h>

namespace gw1k
{


class GuiObject;


class Timer
{

public:

    Timer(double seconds, GuiObject* target, int userdata);

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

    GuiObject* const target;

    const int userdata;

private:

    timeval tvEnd_;

};


} // namespace gw1k

#endif // GW1K_TIMER_H_

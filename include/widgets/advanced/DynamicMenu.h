#ifndef GW1K_DYNAMICMENU_H_
#define GW1K_DYNAMICMENU_H_

#include "../Menu.h"

namespace gw1k
{


class DynamicMenu : public Menu
{

public:

    DynamicMenu(int width = 100,
                const Point& padding = Point(),
                const char* colorScheme = 0);

    ~DynamicMenu();

public:

    void removeEntry(const std::string& text);

    void removeEntry(int token);

    void removeAllEntries();

private:

    typedef Menu super;

};

} // namespace gw1k

#endif // GW1K_DYNAMICMENU_H_


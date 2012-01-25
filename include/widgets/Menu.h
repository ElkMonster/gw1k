#ifndef GW1K_MENU_H_
#define GW1K_MENU_H_

#include "../providers/ActionEventProvider.h"
#include "../listeners/MouseListenerImpl.h"
#include "internal/MenuEntry.h"

#include <string>

namespace gw1k
{


class Menu : public WiBox, public ActionEventProvider, public MouseListenerImpl
{

public:

    Menu(int width = 100,
         const Point& padding = Point(),
         const char* colorScheme = 0);

    ~Menu();

public:

    int addEntry(const std::string& text,
                 int token = -1,
                 bool disabled = false,
                 bool selected = false);

    int getSelectedEntryToken() const;

    const std::string& getSelectedEntryText() const;

    const std::string& getEntryColorSchemeName() const;

    void setTitle(const std::string& title);

    virtual const Point& setSize(float width, float height);

    virtual void mouseClicked(MouseButton b,
                              StateEvent ev,
                              GuiObject* receiver);

    virtual void setColors(const char* colorScheme);

private:

    int getValidToken(int suggestedToken);

    void selectEntry(MenuEntry* newSelectedEntry);

    void repositionEntriesAndResize();

protected:

    Point padding_;

    std::vector<MenuEntry*> entries_;

    MenuEntry* selectedEntry_;

private:

    int unusedToken_;

    std::string sColorScheme_;

    std::string sEntryColorScheme_;

    Label* title_;

    typedef WiBox super;

};

} // namespace gw1k

#endif // GW1K_MENU_H_

#ifndef GW1K_MENU_H_
#define GW1K_MENU_H_

#include "WiBox.h"
#include "Label.h"
#include "../providers/ActionEventProvider.h"
#include "../listeners/MouseListenerImpl.h"

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

    int addEntry(const std::string& text, int token = -1, bool selected = false);

    int getSelectedEntryToken() const;

    const std::string& getSelectedEntryText() const;

    virtual const Point& setSize(float width, float height);

    virtual void mouseClicked(MouseButton b,
                              StateEvent ev,
                              GuiObject* receiver);

    virtual void setColors(const char* colorScheme);

private:

    int getValidToken(int suggestedToken);

    void setEntryColors(Label* entry);

    void selectEntry(Label* newSelectedEntry, int token);

protected:

    Point padding_;

    std::vector<Label*> entries_;

    std::vector<int> entryTokens_;

    Label* selectedEntry_;

    int selectedEntryToken_;

private:

    int unusedToken_;

    std::string sEntryColorScheme_;

    typedef WiBox super;
};

} // namespace gw1k

#endif // GW1K_MENU_H_

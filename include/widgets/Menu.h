#ifndef GW1K_MENU_H_
#define GW1K_MENU_H_

#include "WiBox.h"
#include "Label.h"
#include "../providers/ActionEventProvider.h"
#include "../listeners/MouseListener.h"

#include <string>

namespace gw1k
{


class Menu : public WiBox, public ActionEventProvider, public MouseListener
{

public:

    Menu(int width, const Point& padding, const char* colorScheme = 0);

    ~Menu();

public:

    int addEntry(const std::string& text, int id = -1);

    int getSelectedEntryID() const;

    virtual const Point& setSize(float width, float height);

    virtual void mouseMoved(MouseMovedEvent ev,
                            const Point& pos,
                            const Point& delta,
                            GuiObject* receiver);

    virtual void mouseClicked(MouseButton b,
                              StateEvent ev,
                              GuiObject* receiver);

    virtual void mouseWheeled(int delta, GuiObject* receiver);

    virtual void setColors(const char* colorScheme);

private:

    int getValidID(int suggestedID);

protected:

    Point padding_;

    std::vector<Label*> entries_;

    std::vector<int> entryIDs_;

    Label* selectedEntry_;

    int selectedEntryID_;

private:

    int unusedID_;

    std::string sEntryColorScheme_;

    typedef WiBox super;
};

} // namespace gw1k

#endif // GW1K_MENU_H_

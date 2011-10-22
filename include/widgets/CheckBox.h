#ifndef GW1K_CheckBox_H_
#define GW1K_CheckBox_H_

#include "WiBox.h"
#include "../providers/ActionEventProvider.h"
#include "../listeners/MouseListener.h"
#include "../listeners/ActionListener.h"
#include "Label.h"

namespace gw1k
{


class CheckField;

class CheckBox : public WiBox, public MouseListener, public ActionEventProvider
{

public:

    CheckBox(const Point& pos,
             const Point& size,
             const std::string& text,
             bool checked = false,
             int faceSize = -1,
             const std::string& fontname = "arial.ttf",
             const char* colorScheme = 0);

    virtual ~CheckBox();

public:

    bool isChecked() const;

    void setChecked(bool checked = true);

    virtual void setText(const std::string& text);

    virtual const Point& setSize(float width, float height);

    virtual GuiObject* getContainingObject(const Point& p);

    virtual void setColors(const char* colorScheme);

    virtual void mouseMoved(MouseMovedEvent ev,
                            const Point& pos,
                            const Point& delta,
                            GuiObject* receiver);

    virtual void mouseClicked(MouseButton b,
                              StateEvent ev,
                              GuiObject* receiver);

    virtual void mouseWheeled(int delta, GuiObject* receiver);

private:

    bool checked_;

    CheckField* checkField_;

    Label* label_;

};

} // namespace gw1k

#endif // GW1K_CheckBox_H_

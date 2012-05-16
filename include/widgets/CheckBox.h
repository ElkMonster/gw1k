#ifndef GW1K_CheckBox_H_
#define GW1K_CheckBox_H_

#include "WiBox.h"
#include "../providers/ActionEventProvider.h"
#include "../listeners/MouseListenerImpl.h"
#include "../listeners/ActionListener.h"
#include "Label.h"

namespace gw1k
{


class CheckField;

class CheckBox : public WiBox, public MouseListenerImpl,
    public ActionEventProvider
{

public:

    CheckBox(const Point& pos,
             const Point& size,
             const std::string& text,
             bool checked = false,
             const char* colorScheme = 0);

    virtual ~CheckBox();

public:

    bool isChecked() const;

    void setChecked(bool checked = true);

    void setFontSize(unsigned int size);

    virtual void setText(const std::string& text);

    virtual const Point& setSize(float width, float height);

    virtual GuiObject* getContainingObject(const Point& p);

    virtual void setColors(const char* colorScheme);

    virtual void mouseClicked(MouseButton b,
                              StateEvent ev,
                              GuiObject* receiver);

private:

    bool checked_;

    CheckField* checkField_;

    Label* label_;

};

} // namespace gw1k

#endif // GW1K_CheckBox_H_

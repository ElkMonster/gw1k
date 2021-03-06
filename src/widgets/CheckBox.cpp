#include "widgets/CheckBox.h"

#include <Point2D.h>
#include "ThemeManager.h"
#include "Render.h"

#include <algorithm>

namespace gw1k
{


class CheckField : public WiBox
{

public:

    CheckField(const Point& pos, const Point& size, const CheckBox* checkBox)
    :   WiBox(pos, size),
        checkBox_(checkBox)
    {};

    ~CheckField() {};

    virtual void renderContent(const Point& offset) const
    {
        if (checkBox_->isChecked())
        {
            const Point& pos = offset + getPos();
            int h = getSize().y - 8;
            Color4i* fg, * bg;
            selectColors(fg, bg);
            if (fg)
            {
                setGLColor(fg);
                fillRect(pointToGeomPoint2D(pos + Point(4, 4)),
                    pointToGeomPoint2D(pos + Point(4 + h, 4 + h)));
            }
        }
    };

private:

    const CheckBox* checkBox_;

}; // class CheckField



CheckBox::CheckBox(
    const Point& pos,
    const Point& size,
    const std::string& text,
    bool checked,
    const char* colorScheme)
:   WiBox(pos, size, colorScheme),
    checked_(checked)
{
    checkField_ = new CheckField(Point(3, 3), Point(size.y - 6, size.y - 6), this);
    label_ = new Label(Point(size.y, 1), Point(size.x - size.y - 1, size.y - 2),
                       text);
    addSubObject(checkField_);
    addSubObject(label_);

    checkField_->setEmbedded();
    label_->setEmbedded();

    checkField_->addMouseListener(this);
    label_->setTextProperty(GW1K_ALIGN_LEFT);

    setColors(colorScheme);
}

CheckBox::~CheckBox()
{
    removeAndDeleteSubObject(checkField_);
    removeAndDeleteSubObject(label_);
}


bool
CheckBox::isChecked() const
{
    return checked_;
}


void
CheckBox::setChecked(bool checked)
{
    checked_ = checked;
}


void
CheckBox::setFontSize(unsigned int fontSize)
{
    label_->setFontSize(fontSize);
}

void
CheckBox::setText(const std::string& text)
{
    label_->setText(text);
}


const Point&
CheckBox::setSize(float width, float height)
{
    const Point& newSize = WiBox::setSize(width, height);
    label_->setPos(newSize.y, 1);
    label_->setSize(newSize.x - newSize.y - 2, newSize.y - 2);
    return newSize;
}


void
CheckBox::setColors(const char* colorScheme)
{
    ThemeManager* t = ThemeManager::getInstance();

    std::string baseName(colorScheme ? colorScheme : "CheckBox");
    t->setColors(this, colorScheme, "CheckBox");
    label_->setColors((baseName + ".Label").c_str());
    checkField_->setColors((baseName + ".CheckField").c_str());
}


GuiObject*
CheckBox::getContainingObject(const Point& p)
{
    GuiObject* o = WiBox::getContainingObject(p);
    return o;
}


void
CheckBox::mouseClicked(
    MouseButton b,
    StateEvent ev,
    GuiObject* receiver)
{
    if (receiver == checkField_ && ev == GW1K_PRESSED)
    {
        checked_ = !checked_;

        informActionListeners(this);
    }
}


} // namespace gw1k

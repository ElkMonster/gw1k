#include "widgets/CheckBox.h"

#include <geometry/include/Point2D.h>
#include "ThemeManager.h"
#include "Render.h"

namespace gw1k
{


CheckBox::CheckBox(
    const Point& pos,
    const Point& size,
    const std::string& text,
    bool checked,
    int faceSize,
    const std::string& fontname,
    const char* colorScheme)
:   WiBox(pos, size, colorScheme),
    checked_(checked)
{
    //Point
    //checkField_ = new OGLView(Point(1, 1), Point(size.y - 2, size.y - 2));
    label_ = new Label(Point(size.y, 1), Point(size.x - size.y - 2, size.y - 2),
                       text, faceSize, fontname);
    setColors(colorScheme);
}

CheckBox::~CheckBox()
{
    delete label_;
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
}


GuiObject*
CheckBox::getContainingObject(const Point& p)
{
    GuiObject* o = WiBox::getContainingObject(p);
//    if (o == checkField_)
//    {
        checked_ = !checked_;
//    }
    return this;
}


void
CheckBox::renderFg(const Point& offset) const
{
    WiBox::renderFg(offset);
    Point pos = getPos() + offset;
    int h = getSize().y - 4;
    drawRect(pointToGeomPoint2D(pos + Point(2, 2)),
             pointToGeomPoint2D(pos + Point(2 + h, 2 + h)));
    if (checked_)
    {
        h -= 4;
        fillRect(pointToGeomPoint2D(pos + Point(4, 4)),
                 pointToGeomPoint2D(pos + Point(4 + h, 4 + h)));
    }
}

} // namespace gw1k

#include "widgets/internal/MenuEntry.h"

#include "widgets/Menu.h"

namespace gw1k
{


MenuEntry::MenuEntry(
    const Point& pos,
    const Point& size,
    const std::string& text,
    int token,
    Menu* menu,
    bool disabled,
    bool selected)
:   Label(pos, size, text, false),
    token_(token),
    menu_(menu),
    bIsDisabled_(disabled),
    bIsSelected_(selected)
{
    setColors(0);
}


MenuEntry::~MenuEntry()
{}


int
MenuEntry::getToken() const
{
    return token_;
}


bool
MenuEntry::isDisabled() const
{
    return bIsDisabled_;
}


void
MenuEntry::setDisabled(bool disabled)
{
    bIsDisabled_ = disabled;
    setColors(0);
    setInteractive(!bIsDisabled_);
}


bool
MenuEntry::isSelected() const
{
    return bIsSelected_;
}


void
MenuEntry::setSelected(bool selected)
{
    bIsSelected_ = selected;
    setColors(0);
}


void
MenuEntry::setColors(const char* colorScheme)
{
    const std::string& baseName = menu_->getEntryColorSchemeName();
    if (bIsDisabled_)
    {
        super::setColors((baseName + ".Disabled").c_str());
    }
    else if (bIsSelected_)
    {
        super::setColors((baseName + ".Selected").c_str());
    }
    else
    {
        super::setColors(baseName.c_str());
    }
}


} // namespace gw1k

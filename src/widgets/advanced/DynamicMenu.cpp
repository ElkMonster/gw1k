#include "widgets/advanced/DynamicMenu.h"

#include "WManager.h"

namespace gw1k
{


DynamicMenu::DynamicMenu(int width, const Point& padding, const char* colorScheme)
:   Menu(width, padding, colorScheme)
{}


DynamicMenu::~DynamicMenu()
{}


void
DynamicMenu::removeEntry(const std::string& text)
{
    std::vector<MenuEntry*>::iterator i = entries_.begin();
    while (i != entries_.end())
    {
        if ((*i)->getText() == text)
        {
            if (selectedEntry_ == *i)
            {
                selectedEntry_ = 0;
            }
            WManager::getInstance()->markForDeletion(*i);
            i = entries_.erase(i);
        }
        else
        {
            ++i;
        }
    }
}


void
DynamicMenu::removeEntry(int token)
{
    std::vector<MenuEntry*>::iterator i = entries_.begin();
    while (i != entries_.end())
    {
        if ((*i)->getToken() == token)
        {
            if (selectedEntry_ == *i)
            {
                selectedEntry_ = 0;
            }
            WManager::getInstance()->markForDeletion(*i);
            entries_.erase(i);
            return;
        }
        else
        {
            ++i;
        }
    }
}


void
DynamicMenu::removeAllEntries()
{
    selectedEntry_ = 0;
    WManager* wm = WManager::getInstance();
    for (unsigned int i = 0; i != entries_.size(); ++i)
    {
        wm->markForDeletion(entries_[i]);
    }
    entries_.clear();
    setSize(getSize().x, 0);
}


} // namespace gw1k

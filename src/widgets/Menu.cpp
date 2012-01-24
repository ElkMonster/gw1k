#include "widgets/Menu.h"

namespace gw1k
{


Menu::Menu(int width, const Point& padding, const char* colorScheme)
:   WiBox(Point(), Point(width, 2 * padding.y)),
    padding_(padding),
    selectedEntry_(0),
    selectedEntryID_(-1),
    unusedID_(0)
{
    setColors(colorScheme);
}


Menu::~Menu()
{
    for (unsigned int i = 0; i != entries_.size(); ++i)
    {
        delete entries_[i];
    }
}


int
Menu::addEntry(const std::string& text, int id)
{
    Point predEnd = entries_.empty() ? Point() : entries_.back()->getEnd();
    Point eSize(getSize().x - 2 * padding_.x, 20);
    Label* e = new Label(Point(padding_.x, predEnd.y + padding_.y), eSize, text,
        false, sEntryColorScheme_.c_str());
    e->setFontSize(16);
    e->setPadding(Point(2, 0));
    e->setTextProperty(GW1K_ALIGN_LEFT);
    e->setEmbedded();
    addSubObject(e);
    entries_.push_back(e);
    id = getValidID(id);
    entryIDs_.push_back(id);

    const Point& size = getSize();
    setSize(size.x, size.y + eSize.y);

    return id;
}


const Point&
Menu::setSize(float width, float height)
{
    int h = 2 * padding_.y;
    int entryH = 0;
    if (!entries_.empty())
    {
        h = entries_.back()->getEnd().y + padding_.y;
        entryH = entries_.back()->getSize().y;
    }
    const Point& newSize = super::setSize(width, h);

    Point entrySize(newSize.x - 2 * padding_.x, entryH);
    for (unsigned int i = 0; i != entries_.size(); ++i)
    {
        entries_[i]->setSize(entrySize.x, entrySize.y);
    }

    return newSize;
}


void
Menu::mouseMoved(
    MouseMovedEvent ev,
    const Point& pos,
    const Point& delta,
    GuiObject* receiver)
{
}


void
Menu::mouseClicked(MouseButton b, StateEvent ev, GuiObject* receiver)
{

}


void
Menu::mouseWheeled(int delta, GuiObject* receiver)
{

}


void
Menu::setColors(const char* colorScheme)
{
    std::string sColorScheme(colorScheme ? colorScheme : "Menu");
    super::setColors(sColorScheme.c_str());

    sEntryColorScheme_ = std::string(sColorScheme + ".Entry");
    for (unsigned int i = 0; i != entries_.size(); ++i)
    {
        if (entryIDs_[i] == selectedEntryID_)
        {
            entries_[i]->setColors((sEntryColorScheme_ + ".Selected").c_str());
        }
        else
        {
            entries_[i]->setColors(sEntryColorScheme_.c_str());
        }
    }
}


int
Menu::getValidID(int suggestedID)
{
    int id = (suggestedID < 0) ? unusedID_ : suggestedID;

    bool idOk = false;
    while (!idOk)
    {
        idOk = true;
        for (unsigned int i = 0; i != entryIDs_.size(); ++i)
        {
            if (id == entryIDs_[i])
            {
                id = ++unusedID_;
                idOk = false;
                break;
            }
        }
    }

    ++unusedID_;
    return id;
}


} // namespace gw1k

#include "widgets/Menu.h"

namespace gw1k
{


Menu::Menu(int width, const Point& padding, const char* colorScheme)
:   WiBox(Point(), Point(width, 2 * padding.y)),
    padding_(padding),
    selectedEntry_(0),
    selectedEntryToken_(-1),
    unusedToken_(0)
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
Menu::addEntry(const std::string& text, int token, bool selected)
{
    Point predEnd = entries_.empty() ? Point() : entries_.back()->getEnd();
    Point eSize(getSize().x - 2 * padding_.x, 20);
    Label* e = new Label(Point(padding_.x, predEnd.y + padding_.y), eSize, text);
    if (selected)
    {
        selectEntry(e, token);
    }
    setEntryColors(e);

    e->setFontSize(16);
    e->setPadding(Point(2, 0));
    e->setTextProperty(GW1K_ALIGN_LEFT);
    e->setEmbedded();
    e->addMouseListener(this);
    addSubObject(e);
    entries_.push_back(e);
    token = getValidToken(token);
    entryTokens_.push_back(token);

    const Point& size = getSize();
    setSize(size.x, size.y + eSize.y);

    return token;
}


int
Menu::getSelectedEntryToken() const
{
    return selectedEntryToken_;
}


const std::string&
Menu::getSelectedEntryText() const
{
    return selectedEntry_->getTextWidget().getText();
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
Menu::mouseClicked(MouseButton b, StateEvent ev, GuiObject* receiver)
{
    for (unsigned int i = 0; i != entries_.size(); ++i)
    {
        if (receiver == entries_[i])
        {
            selectEntry(entries_[i], entryTokens_[i]);
            informActionListeners(this);
            return;
        }
    }
}


void
Menu::setColors(const char* colorScheme)
{
    std::string sColorScheme(colorScheme ? colorScheme : "Menu");
    super::setColors(sColorScheme.c_str());

    sEntryColorScheme_ = std::string(sColorScheme + ".Entry");
    for (unsigned int i = 0; i != entries_.size(); ++i)
    {
        setEntryColors(entries_[i]);
    }
}


int
Menu::getValidToken(int suggestedToken)
{
    int token = (suggestedToken < 0) ? unusedToken_ : suggestedToken;

    bool tokenOk = false;
    while (!tokenOk)
    {
        tokenOk = true;
        for (unsigned int i = 0; i != entryTokens_.size(); ++i)
        {
            if (token == entryTokens_[i])
            {
                token = ++unusedToken_;
                tokenOk = false;
                break;
            }
        }
    }

    ++unusedToken_;
    return token;
}


void
Menu::setEntryColors(Label* entry)
{
    if (entry == selectedEntry_)
    {
        entry->setColors((sEntryColorScheme_ + ".Selected").c_str());
    }
    else
    {
        entry->setColors(sEntryColorScheme_.c_str());
    }
}


void
Menu::selectEntry(Label* newSelectedEntry, int token)
{
    if (selectedEntry_ != newSelectedEntry)
    {
        Label* prevSelectedEntry = selectedEntry_;
        selectedEntry_ = newSelectedEntry;
        selectedEntryToken_ = token;

        setEntryColors(selectedEntry_);
        if (prevSelectedEntry)
        {
            setEntryColors(prevSelectedEntry);
        }
    }
}


} // namespace gw1k

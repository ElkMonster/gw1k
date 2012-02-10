#include "widgets/Menu.h"

namespace gw1k
{


Menu::Menu(int width, const Point& padding, const char* colorScheme)
:   WiBox(Point(), Point(width, 2 * padding.y)),
    padding_(padding),
    selectedEntry_(0),
    unusedToken_(0),
    title_(0)
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
Menu::addEntry(const std::string& text, int token, bool disabled, bool selected)
{
    int y = entries_.empty()
        ? (title_ ? title_->getEnd().y : 0) : entries_.back()->getEnd().y;
    Point ePos(padding_.x, y + padding_.y);
    Point eSize(getSize().x - 2 * padding_.x, 20);
    MenuEntry* e = new MenuEntry(ePos, eSize, text, getValidToken(token), this,
        disabled, selected);
    if (selected)
    {
        selectEntry(e);
    }

    e->setFontSize(12);
    e->setPadding(Point(2, 0));
    e->setTextProperty(GW1K_ALIGN_LEFT);
    e->setEmbedded();
    e->addMouseListener(this);
    addSubObject(e);
    entries_.push_back(e);

    const Point& size = getSize();
    setSize(size.x, size.y + eSize.y);

    return token;
}


int
Menu::getSelectedEntryToken() const
{
    return selectedEntry_->getToken();
}


const std::string&
Menu::getSelectedEntryText() const
{
    return selectedEntry_->getTextWidget().getText();
}


const std::string&
Menu::getEntryColorSchemeName() const
{
    return sEntryColorScheme_;
}


void
Menu::setTitle(const std::string& title)
{
    if (!title.empty())
    {
        if (title_)
        {
            title_->setText(title);
        }
        else
        {
            Point size(getSize().x - 2 * padding_.x, 20);
            title_ = new Label(padding_, size, title, false,
                (sColorScheme_ + ".Title").c_str());
            title_->setInteractive(false);
            title_->setFontSize(12);
            addSubObject(title_);
            repositionEntriesAndResize();
        }
    }
    else // title == ""
    {
        if (title_)
        {
            removeSubObject(title_);
            title_ = 0;
            repositionEntriesAndResize();
        }
    }
}


const Point&
Menu::setSize(float width, float height)
{
    int h = (title_ ? title_->getSize().y : 0) + 2 * padding_.y;
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
        if ((receiver == entries_[i]) && (ev == GW1K_PRESSED))
        {
            selectEntry(entries_[i]);
            informActionListeners(this);
            return;
        }
    }
}


void
Menu::setColors(const char* colorScheme)
{
    sColorScheme_ = colorScheme ? colorScheme : "Menu";
    super::setColors(sColorScheme_.c_str());

    sEntryColorScheme_ = std::string(sColorScheme_ + ".Entry");
}


int
Menu::getValidToken(int suggestedToken)
{
    int token = (suggestedToken < 0) ? unusedToken_ : suggestedToken;

    bool tokenOk = false;
    while (!tokenOk)
    {
        tokenOk = true;
        for (unsigned int i = 0; i != entries_.size(); ++i)
        {
            if (token == entries_[i]->getToken())
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
Menu::selectEntry(MenuEntry* newSelectedEntry)
{
    if (selectedEntry_ != newSelectedEntry)
    {
        MenuEntry* prevSelectedEntry = selectedEntry_;
        selectedEntry_ = newSelectedEntry;
        selectedEntry_->setSelected();

        if (prevSelectedEntry)
        {
            prevSelectedEntry->setSelected(false);
        }
    }
}


void
Menu::repositionEntriesAndResize()
{
    Point pos(padding_.x, (title_ ? title_->getEnd().y : 0) + padding_.y);
    for (unsigned int i = 0; i != entries_.size(); ++i)
    {
        entries_[i]->setPos(pos.x, pos.y);
        pos.y += entries_[i]->getSize().y + padding_.y;
    }
    super::setSize(getSize().x, pos.y);
}


} // namespace gw1k

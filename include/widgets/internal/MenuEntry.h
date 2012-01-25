#ifndef GW1K_MENUENTRY_H_
#define GW1K_MENUENTRY_H_

#include "../Label.h"

namespace gw1k
{


class Menu;


class MenuEntry : public Label
{

public:

    MenuEntry(const Point& pos,
              const Point& size,
              const std::string& text,
              int token,
              Menu* menu,
              bool disabled = false,
              bool selected = false);

    ~MenuEntry();

public:

    int getToken() const;

    bool isDisabled() const;

    void setDisabled(bool disabled = true);

    bool isSelected() const;

    void setSelected(bool selected = true);

    virtual void setColors(const char* colorScheme);

private:

    int token_;

    Menu* menu_;

    bool bIsDisabled_;

    bool bIsSelected_;

    typedef Label super;

};


} // namespace gw1k

#endif // GW1K_MENUENTRY_H_

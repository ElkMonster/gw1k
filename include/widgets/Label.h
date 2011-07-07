#ifndef GW1K_LABEL_H_
#define GW1K_LABEL_H_

#include "WiBox.h"
#include "../listeners/MouseListener.h"
#include "Text.h"
#include "../Gw1kConstants.h"

#include <string>

namespace gw1k
{


class Label : public WiBox, public MouseListener
{

public:

    Label(const Point& pos,
          const Point& size,
          const std::string& text,
          int faceSize = -1,
          const std::string& fontname = "arial.ttf",
          const char* colorScheme = 0);

    virtual ~Label();

public:

    virtual void preRenderUpdate();

    virtual void setText(const std::string& text);

    virtual const Point& setSize(float width, float height);

    void setTextProperty(TextProperty p);

    Text& getTextWidget();

    virtual void mouseMoved(MouseMovedEvent ev,
                            const Point& pos,
                            const Point& delta,
                            GuiObject* target);

    virtual void mouseClicked(MouseButton b,
                              StateEvent ev,
                              GuiObject* target);

    virtual void mouseWheeled(int delta, GuiObject* target);

    void setColors(const char* colorScheme);

private:

    void updateVerticalAlignment();

    bool textProp(TextProperty p) const;

private:

    Text text_;

    int textProps_;

};


} // namespace gw1k

#endif // GW1K_LABEL_H_

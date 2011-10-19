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
          bool autoSize = false,
          const char* colorScheme = 0);

    virtual ~Label();

public:

    /**
     * When enabling auto-size, the text is reset to one line. Use
     * setLineLength() to set the length at which lines wrap.
     *
     */
    void setAutoSized(bool b = true);

    bool isAutoSized() const;

    /**
     * Set the line length. Text that uses more space than the specified line
     * length is wrapped. Negative length values disable wrapping.
     * The includesPadding argument states whether the given length already
     * includes horizontal padding. If true, the actual line length is
     * calculated internally by subtracting padding. If false, the line is set
     * to have the given length.
     */
    void setLineLength(int length, bool includesPadding = true);

    virtual void setText(const std::string& text);

    void setPadding(const Point& padding);

    const Point& getPadding() const;

    virtual const Point& setSize(float width, float height);

    void setTextProperty(TextProperty p);

    Text& getTextWidget();

    virtual void mouseMoved(MouseMovedEvent ev,
                            const Point& pos,
                            const Point& delta,
                            GuiObject* receiver);

    virtual void mouseClicked(MouseButton b,
                              StateEvent ev,
                              GuiObject* receiver);

    virtual void mouseWheeled(int delta, GuiObject* receiver);

    void setColors(const char* colorScheme);

private:

    const Point& setSizeInternal(float width, float height);

    void updateTextAlignment();

    bool textProp(TextProperty p) const;

    void adaptToTextSize();

private:

    Box textBox_;

    Point padding_;

    int textProps_;

    bool bAutoSized_;

    Text text_;

    /**
     * If true, a vertically centred Label's text is placed below the
     * mathematical centre to improve the perceived "centring". If false, the
     * text is placed at the mathematical centre, but it may seem to be offset
     * to the top by a little due to most letters "bodies" lying mostly above
     * the baseline (except for letters like g, j, y, etc.).
     * Default value is true.
     */
    bool bVCenterVisually_;

    int lineLength_;

};


} // namespace gw1k

#endif // GW1K_LABEL_H_

#ifndef GW1K_LABEL_H_
#define GW1K_LABEL_H_

#include "WiBox.h"
#include "../listeners/MouseListener.h"
#include "internal/Text.h"
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
          bool autoSize = false,
          const char* colorScheme = 0);

    virtual ~Label();

public:

    /**
     * When enabling auto-size, the text is reset to one line. Calls to
     * setSize() will only
     * setLineLength() to set the length at which lines wrap.
     *
     */
    void setAutoSized(bool b = true);

    bool isAutoSized() const;

    /**
     * Enables or disables text wrapping. When enabled, the text will be wrapped
     * if it exceeds the available horizontal space in a non-auto-sized Label.
     * Wrapping can be combined with auto-sizing: If auto-sizing and wrapping
     * are enabled, setSize()'s width argument will determine the line length.
     */
    void setWrapText(bool wrap = true);

    virtual void setText(const std::string& text);

    /**
     * If fontSize is -1, the actual font size used is calculated based on the
     * current height of the widget. Any widget size changes after this call do
     * not modify font size.
     */
    void setFontSize(int fontSize);

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

    void updateTextAlignment();

    bool textProp(TextProperty p) const;

    void adaptToTextSize();

    void updateWrappedTextLayout();

    unsigned int calculateFaceSize(int fontSize) const;

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

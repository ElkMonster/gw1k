#ifndef GW1K_SCROLLPANE_H_
#define GW1K_SCROLLPANE_H_

#include "Box.h"
#include "WiBox.h"
#include "../listeners/SliderListener.h"
#include "Slider.h"
#include "ClippingBox.h"


namespace gw1k
{


class ScrollPane : public WiBox, public SliderListener
{

public:

    ScrollPane(const Point& pos,
               const Point& size,
               bool autoSize = true,
               const char* colorScheme = 0);

    ~ScrollPane();

public:

    virtual const Point& setSize(float width, float height);

    virtual void addSubObject(GuiObject* o);

    virtual void removeSubObject(GuiObject* o);

    virtual void sliderValueChanged(Slider* slider, float newVal, float delta);

    void refreshLayout();

    virtual void setColors(const char* colorScheme);

    const Point& getVisibleSize() const;

protected:

    void updateSliders();

protected:

    ClippingBox pane_;

    Slider hSlider_;

    Slider vSlider_;

};

} // namespace gw1k

#endif // GW1K_SCROLLPANE_H_

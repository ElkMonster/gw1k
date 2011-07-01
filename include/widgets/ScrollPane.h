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
               const char* colorScheme = 0,
               bool autoSize = true,
               bool stickySliders = false);

    ~ScrollPane();

public:

    virtual const Point& setSize(float width, float height);

    virtual void addSubObject(GuiObject* o);

    virtual void removeSubObject(GuiObject* o);

    virtual void sliderValueChanged(Slider* slider, float newVal, float delta);

    /**
     * Rearranges the layout of the ScrollPane to honor changed sub-object sizes.
     *
     * This method should be called when sub-objects have been resized (but not
     * added or removed). The method will then calculate the new real size of
     * the pane and set up the sliders accordingly.
     */
    void refreshLayout();

    virtual void setColors(const char* colorScheme);

    const Point& getVisibleSize() const;

    Slider& getHSlider();

    Slider& getVSlider();

protected:

    void resizePaneAndSliders();

    void updateSliders();

    void updateSlider(Slider* slider,
                      int realSize,
                      int paneSize,
                      int offset,
                      int range);

    /**
     * Calls resizePaneAndSliders() and updateSliders().
     *
     * accommodationStateChanged should be the result of a comparison of two
     * values obtained via ClippingBox::getAccommodationState().
     * resizePaneAndSliders() will only be called if accommodationStateChanged
     * is true.
     */
    void revalidatePaneAndSliders(bool accommodationStateChanged);


protected:

    ClippingBox* pane_;

    Slider* hSlider_;

    Slider* vSlider_;

    bool stickySliders_;

};

} // namespace gw1k

#endif // GW1K_SCROLLPANE_H_

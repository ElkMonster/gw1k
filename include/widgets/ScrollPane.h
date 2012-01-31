#ifndef GW1K_SCROLLPANE_H_
#define GW1K_SCROLLPANE_H_

#include "Box.h"
#include "WiBox.h"
#include "../listeners/ActionListener.h"
#include "Slider.h"


namespace gw1k
{


class ClippingBox;

/**
 * ScrollPane can auto-resize sub-objects to let them fit into the ScrollPane in
 * one dimension. When specifying ADJUST_HORIZ or ADJUST_VERT, the sub-objects'
 * width or height, respectively, will be adjusted such that it will never
 * exceed the ScrollPane's size in that dimension. This means that no slider
 * will ever appear for that dimension. If a slider appears for the other
 * dimension (which will reduce the size of the auto-adjusted dimension),
 * sub-object size will be reduced accordingly. Likewise, the size will increase
 * when the other dimension's slider disappears. Also, the sub-objects will grow
 * and shrink when the ScrollPane's size is changed.
 * When in auto-adjust mode, the sub-objects' coordinate of the auto-adjusted
 * dimension should be positive and in the range of the ScrollPane's size.
 * Positions will not be changed by the auto-adjust mechanism, only sizes will
 * be adjusted to align with the right or bottom edge, respectively.
 * Generally, it is recommended to use ScrollPane auto-adjust modes with
 * sub-objects which are intended to fully fill the ScrollPane in the according
 * dimension.
 */
class ScrollPane : public WiBox, public ActionListener
{

public:

    enum AutoAdjustSize { ADJUST_HORIZ, ADJUST_VERT, ADJUST_NONE };

    /**
     * @param stickySliders if true, sliders will always be shown
     */
    ScrollPane(const Point& pos,
               const Point& size,
               const char* colorScheme = 0,
               AutoAdjustSize autoAdjustSize = ADJUST_NONE,
               bool stickySliders = false);

    ~ScrollPane();

public:

    /**
     * Sets the ScrollPane size, and updates sub-objects and sliders
     * accordingly.
     */
    virtual const Point& setSize(float width, float height);

    /**
     * Adds a sub-object, and resizes the internal pane if required to fit the
     * sub-object by calling revalidatePaneAndSliders().
     */
    virtual void addSubObject(GuiObject* o);

    /**
     * Removes a sub-object, and resizes the internal pane if less space than
     * before is required to fit the sub-objects by calling
     * revalidatePaneAndSliders().
     */
    virtual void removeSubObject(GuiObject* o);

    virtual void actionPerformed(GuiObject* receiver);

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

    /**
     * Sets the width of the vertical and the height of the horizontal slider.
     */
    void setSliderSize(int size);

    /**
     * Sets the margin between sliders and content. It is only applied if the
     * according slider is visible.
     */
    void setSliderMargin(int margin);

protected:

    /**
     * Checks whether sliders need to be visible (if not always visible due to
     * the stickySliders option), resizes the internal pane to make room for
     * them, and sets the sliders' location and size accordingly.
     *
     * Resizing the internal pane will auto-resize the sub-objects if enabled
     * (see constructor).
     */
    void resizePaneAndSliders();

    /**
     * Updates the horizontal and vertical sliders via updateSlider().
     */
    void updateSliders();

    /**
     * Updates the given slider to reflect the state of the ScrollPane in the
     * according dimension.
     *
     * Updates include enabling or disabling the slider depending on the
     * realSize to paneSize proportion, setting visibility (if not made always
     * visible with the stickySliders option, see constructor), and setting
     * handle size and slider value (and thus, implicitly, handle position).
     */
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

    /**
     * The width of the vertical and the height of the horizontal slider in
     * pixels.
     */
    int sliderSize_;

    /**
     * The space that is kept between sliders and pane_ in pixels.
     */
    int sliderMargin_;

    ClippingBox* pane_;

    Slider* hSlider_;

    Slider* vSlider_;

    bool stickySliders_;

};

} // namespace gw1k

#endif // GW1K_SCROLLPANE_H_

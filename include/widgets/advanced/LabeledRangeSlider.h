#ifndef GW1K_LABELEDRANGESLIDER_H_
#define GW1K_LABELEDRANGESLIDER_H_

#include "../Box.h"
#include "../RangeSlider.h"
#include "../Label.h"
#include "../../providers/ActionEventProvider.h"
#include "../../listeners/ActionListener.h"

namespace gw1k
{


class LabeledRangeSlider : public Box, public ActionEventProvider,
    public ActionListener
{

public:

    LabeledRangeSlider(const Point& pos,
                       const Point& size,
                       const float range[2],
                       RangeSlider::RangeType rangeTye = RangeSlider::MAP_LINEAR,
                       const char* colorScheme = 0);

    ~LabeledRangeSlider();

public:

    void setLValue(float v);

    float getLValue() const;

    void setRValue(float v);

    float getRValue() const;

    virtual void actionPerformed(GuiObject* receiver);

    void setColors(const char* colorScheme);

    void render(const Point& offset) const;

protected:

    void updateLabels();

    void setup();

private:

    RangeSlider* slider_;

    Label* lLabel_;

    Label* rLabel_;

};


} // namespace gw1k


#endif // GW1K_LABELEDRANGESLIDER_H_

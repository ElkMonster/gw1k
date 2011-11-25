#ifndef GW1K_LABELEDRANGESLIDER_H_
#define GW1K_LABELEDRANGESLIDER_H_

#include "../Box.h"
#include "../RangeSlider.h"
#include "NumberLabel.h"
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
                       MapType mapTye = MAP_LINEAR,
                       const char* colorScheme = 0);

    ~LabeledRangeSlider();

public:

    void setLValue(float v);

    float getLValue() const;

    void setRValue(float v);

    float getRValue() const;

    void setLabelPrecision(int left, int right);

    virtual void actionPerformed(GuiObject* receiver);

    virtual const Point& setSize(float width, float height);

    void setColors(const char* colorScheme);

protected:

    void updateLabels();

    void setup();

private:

    RangeSlider* slider_;

    NumberLabel* lLabel_;

    NumberLabel* rLabel_;

};


} // namespace gw1k


#endif // GW1K_LABELEDRANGESLIDER_H_

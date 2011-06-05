#ifndef GW1K_SLIDERLISTENER_H_
#define GW1K_SLIDERLISTENER_H_


namespace gw1k
{

class Slider;

class SliderListener
{

public:

    virtual void sliderValueChanged(Slider* slider, float newVal, float delta) = 0;

};


} // namespace gw1k

#endif // GW1K_SLIDERLISTENER_H_

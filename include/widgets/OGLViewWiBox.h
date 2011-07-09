#ifndef GW1K_OGLVIEWWIBOX_H_
#define GW1K_OGLVIEWWIBOX_H_

#include "WiBox.h"
#include "OGLView.h"


namespace gw1k
{


class OGLViewWiBox_OGLRenderer;


class OGLViewWiBox : public WiBox
{

public:

    OGLViewWiBox(const Point& pos, const Point& size, const char* colorScheme);

    virtual ~OGLViewWiBox();

public:

    virtual const Point& setSize(float width, float height);

    virtual void renderOGLContent() const = 0;

private:

    OGLViewWiBox_OGLRenderer* oglRenderer_;

};


} // namespace gw1k

#endif // GW1K_OGLVIEWWIBOX_H_

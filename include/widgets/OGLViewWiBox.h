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

    void setTranslation(const geom::Point2D& t);

    const geom::Point2D& getTranslation() const;

    void setZoomFactor(float z);

    float getZoomFactor() const;

    void relativeTranslateBy(const Point& delta);

    void allowMouseControl(bool enabled = true);

    virtual const Point& setSize(float width, float height);

    virtual void renderOGLContent() const = 0;

    geom::Point2D pxToGLPos(const Point& relPos) const;

    geom::Point2D pxToGLDelta(const Point& delta) const;

    geom::Point2D pxToGLUnit(const Point& v) const;

    Point glPosToPx(const geom::Point2D& pos) const;

    void setShadeColors(const ColorTable& colorTable);

    void setShadeColorScheme(const char* colorScheme);

private:

    OGLViewWiBox_OGLRenderer* oglRenderer_;

};


} // namespace gw1k

#endif // GW1K_OGLVIEWWIBOX_H_

#ifndef GW1K_TEXTUREWIBOX_H_
#define GW1K_TEXTUREWIBOX_H_

#include "WiBox.h"
#include "TextureView.h"

namespace gw1k
{


class TextureWiBox : public WiBox
{

public:

    /**
     * If colorScheme is given, the enclosed TextureView's shadeColorScheme
     * path defaults to <colorScheme>.Shade.
     */
    TextureWiBox(const Point& pos,
                 const Point& size,
                 const std::string& filename,
                 bool bResizeToImageSize = false,
                 const char* colorScheme = 0);

    virtual ~TextureWiBox();

public:

    void setPadding(const Point& padding);

    void setAspectRatioAutoResize(TextureView::AspectRatioAutoAdapt a);

    bool loadTexture(const std::string& filename);

    void setShadeColors(const ColorTable& colorTable);

    void setShadeColorScheme(const char* colorScheme);

    const Point& getTextureSize() const;

    virtual const Point& setSize(float width, float height);

protected:

    TextureView* texView_;

private:

    Point padding_;

};


} // namespace gw1k

#endif // GW1K_TEXTUREWIBOX_H_

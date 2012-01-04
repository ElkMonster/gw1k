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
     * If colorScheme is given, the enclosed TextureView's texMulColorScheme
     * path defaults to <colorScheme>.TexMul.
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

    void setTexMulColors(const ColorTable& colorTable);

    void setTexMulColorScheme(const char* colorScheme);

    const Point& getTextureSize() const;

    virtual const Point& setSize(float width, float height);

private:

    Point padding_;

    TextureView* texView_;

};


} // namespace gw1k

#endif // GW1K_TEXTUREWIBOX_H_

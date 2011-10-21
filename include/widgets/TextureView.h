#ifndef GW1K_TEXTUREVIEW_H_
#define GW1K_TEXTUREVIEW_H_

#include "OGLView.h"

#include "../Color4i.h"

#include <GL/gl.h>

namespace gw1k
{


/**
 * MouseListener methods are implemented to disable OGLView's default
 * implementation that allows to translate and zoom into the coordinate system.
 * Use allowMouseControl() to enable it.
 */
class TextureView : public OGLView
{

public:

    enum AspectRatioAutoAdapt { AR_NO_ADAPT, AR_ADAPT_WIDTH, AR_ADAPT_HEIGHT };

    TextureView(const Point& pos, const Point& size, const std::string& filename);

    virtual ~TextureView();

public:

    /**
     * Determines the way the widget behaves upon setSize().
     *
     * AR_NO_ADAPT resizes the widget to the given size. The texture is scaled
     * in both dimensions to fit the size.
     * AR_ADAPT_WIDTH resizes the widget to the given height and sets width to a
     * value such that the original image aspect ratio is kept.
     * AR_ADAPT_HEIGHT resizes the widget to the given width and sets height to
     * a value such that the original image aspect ratio is kept.
     */
    void setAspectRatioAutoResize(AspectRatioAutoAdapt a);

    /**
     * Loads the given image file and enqueues this TextureView in WManager's
     * preRenderUpdate() list. Thus, a texture will only be created from the
     * image when the OpenGL state is ready for it.
     * (See http://www.opengl.org/wiki/Common_Mistakes for details)
     */
    bool loadTexture(const std::string& filename);

    /**
     * Creates a texture from an image loaded by loadTexture(). This should only
     * be called after the OpenGL context has been created. See loadTexture()
     * for information on this.
     */
    void createTexture();

    void setTexMulColor(const Color4i* c);

    virtual void preRenderUpdate();

    virtual void renderOGLContent() const;

    virtual const Point& setSize(float width, float height);

private:

    std::string filename_;

    GLuint* pTex_;

    int imgWidth_;

    int imgHeight_;

    bool imgAlpha_;

    unsigned char* pImgData_;

    /** If this is true, a call to preRenderUpdate() will create the texture */
    bool bReqLoadTexture_;

    Color4i texMulColor_;

    AspectRatioAutoAdapt aspectRatioAutoAdapt_;
};


} // namespace gw1k

#endif // GW1K_TEXTUREVIEW_H_


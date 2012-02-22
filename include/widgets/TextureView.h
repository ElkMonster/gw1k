#ifndef GW1K_TEXTUREVIEW_H_
#define GW1K_TEXTUREVIEW_H_

#include "OGLView.h"

#ifdef __APPLE__
#include <OpenGL/glew.h>
#else
#include <GL/glew.h>
#endif

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

    /**
     * If bResizeToImageSize is true, then size is ignored and the image's size
     * is used. This is only a convenience feature of the constructor; the
     * widget can be resized as usual.
     */
    TextureView(const Point& pos,
                const Point& size,
                const std::string& filename,
                bool bResizeToImageSize = false,
                const char* shadeColorScheme = 0);

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
     * Loads the given image file and generates a texture for it.
     */
    bool loadTexture(const std::string& filename);

    const Point& getTextureSize() const;

    virtual void renderOGLContent() const;

    virtual const Point& setSize(float width, float height);

protected:

    /**
     * Creates a texture from an image loaded by loadTexture(). This should only
     * be called after the OpenGL context has been created. See loadTexture()
     * for information on this.
     */
    void createTexture();

private:

    std::string filename_;

    GLuint* pTex_;

    Point imgSize_;

    bool imgAlpha_;

    unsigned char* pImgData_;

    AspectRatioAutoAdapt aspectRatioAutoAdapt_;
};


} // namespace gw1k

#endif // GW1K_TEXTUREVIEW_H_


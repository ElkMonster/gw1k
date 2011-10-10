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

    TextureView(const Point& pos, const Point& size, const std::string& filename);

    virtual ~TextureView();

public:

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

    void allowMouseControl(bool enabled = true);

    virtual void preRenderUpdate();

    virtual void renderOGLContent() const;

    virtual void mouseMoved(MouseMovedEvent ev,
                            const Point& pos,
                            const Point& delta,
                            GuiObject* receiver);

    virtual void mouseClicked(MouseButton b,
                              StateEvent ev,
                              GuiObject* receiver);

    virtual void mouseWheeled(int delta, GuiObject* receiver);

private:

    std::string filename_;

    GLuint* pTex_;

    int imgWidth_;

    int imgHeight_;

    bool imgAlpha_;

    unsigned char* pImgData_;

    /** If this is true, a call to preRenderUpdate() will create the texture */
    bool bReqLoadTexture_;

    bool bMouseControl_;

    Color4i texMulColor_;
};


} // namespace gw1k

#endif // GW1K_TEXTUREVIEW_H_


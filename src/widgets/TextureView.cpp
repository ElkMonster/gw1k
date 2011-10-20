#include "widgets/TextureView.h"

#include "utils/PNGLoader.h"
#include "WManager.h"
#include "Render.h"
#include "Log.h"

#include <cstring>

namespace gw1k
{


TextureView::TextureView(
    const Point& pos,
    const Point& size,
    const std::string& filename)
:   OGLView(pos, size),
    filename_(filename),
    pTex_(0),
    pImgData_(0),
    bReqLoadTexture_(false),
    bMouseControl_(false),
    texMulColor_(255, 255, 255, 255)
{
    loadTexture(filename);
}


TextureView::~TextureView()
{
    if (pTex_)
    {
        glDeleteTextures(1, pTex_);
    }
}


bool
TextureView::loadTexture(const std::string& filename)
{
    if (pTex_)
    {
        glDeleteTextures(1, pTex_);
        delete pTex_;
    }

    if (loadPngTexture(filename.c_str(), imgWidth_, imgHeight_, imgAlpha_, pImgData_))
    {
        if (imgWidth_ != imgHeight_)
        {
            if (!std::strstr(reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)),
                "GL_ARB_texture_non_power_of_two"))
            {
                Log::error("TextureView", Log::os()
                    << "Image size is " << imgWidth_ << "x" << imgHeight_
                    << ", but GL_ARB_texture_non_power_of_two not supported");
            }
        }

        bReqLoadTexture_ = true;
        WManager::getInstance()->registerForPreRenderUpdate(this);
        return true;
    }
    return false;
}


void
TextureView::createTexture()
{
    if (pImgData_)
    {
        pTex_ = new GLuint(0);

        glGenTextures(1, pTex_);
        glBindTexture(GL_TEXTURE_2D, *pTex_);

        // Required for some graphics cards to work with non-power of two images
        // according to http://stackoverflow.com/questions/318194/
        // display-arbitrary-size-2d-image-in-opengl/413658#413658
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        int format = imgAlpha_ ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, imgWidth_, imgHeight_, 0, format,
            GL_UNSIGNED_BYTE, pImgData_);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        delete[] pImgData_;
    }
}


void
TextureView::setTexMulColor(const Color4i* c)
{
    if (c)
    {
        texMulColor_ = *c;
    }
    else
    {
        texMulColor_ = Color4i(0, 0, 0, 0);
    }
}


void
TextureView::allowMouseControl(bool state)
{
    bMouseControl_ = state;
}


void
TextureView::preRenderUpdate()
{
    if (bReqLoadTexture_)
    {
        createTexture();
        bReqLoadTexture_ = false;
    }
}

void
TextureView::renderOGLContent() const
{
    // Reset colour because texture's colour values are multiplied with the
    // current colour
    setGLColor(&texMulColor_);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, *pTex_);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-1.0, -1.0, 0.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-1.0, 1.0, 0.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(1.0, 1.0, 0.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(1.0, -1.0, 0.0);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void
TextureView::mouseMoved(
    MouseMovedEvent ev,
    const Point& pos,
    const Point& delta,
    GuiObject* receiver)
{
    if (bMouseControl_)
    {
        OGLView::mouseMoved(ev, pos, delta, receiver);
    }
}


void
TextureView::mouseClicked(MouseButton b, StateEvent ev, GuiObject* receiver)
{
    if (bMouseControl_)
    {
        OGLView::mouseClicked(b, ev, receiver);
    }
}


void
TextureView::mouseWheeled(int delta, GuiObject* receiver)
{
    if (bMouseControl_)
    {
        OGLView::mouseWheeled(delta, receiver);
    }
}


} // namespace gw1k

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
    const std::string& filename,
    bool bResizeToImageSize,
    const char* shadeColorScheme)
:   OGLView(pos, size, shadeColorScheme),
    filename_(filename),
    pTex_(0),
    imgSize_(0, 0),
    pImgData_(0),
    aspectRatioAutoAdapt_(AR_NO_ADAPT)
{
    loadTexture(filename);
    if (bResizeToImageSize)
    {
        GuiObject::setSize(imgSize_.x, imgSize_.y);
    }
}


TextureView::~TextureView()
{
    if (pTex_)
    {
        glDeleteTextures(1, pTex_);
        delete pTex_;
    }
}


void
TextureView::setAspectRatioAutoResize(AspectRatioAutoAdapt a)
{
    if (a != aspectRatioAutoAdapt_)
    {
        aspectRatioAutoAdapt_ = a;
        const Point& size = getSize();
        setSize(size.x, size.y);
    }
}


bool
TextureView::loadTexture(const std::string& filename)
{
    if (pTex_)
    {
        glDeleteTextures(1, pTex_);
        delete pTex_;
        pTex_ = 0;
    }

    if (loadPngTexture(filename.c_str(), imgSize_.x, imgSize_.y, imgAlpha_, pImgData_))
    {
        if (imgSize_.x != imgSize_.y)
        {
            if (!std::strstr(reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)),
                "GL_ARB_texture_non_power_of_two"))
            {
                Log::error("TextureView", Log::os()
                    << "Image size is " << imgSize_.x << "x" << imgSize_.y
                    << ", but GL_ARB_texture_non_power_of_two not supported");
            }
        }

        createTexture();
        return true;
    }
    return false;
}


void
TextureView::createTexture()
{
    if (pImgData_)
    {
        DELETE_PTR(pTex_);
        pTex_ = new GLuint(0);

        glGenTextures(1, pTex_);
        glBindTexture(GL_TEXTURE_2D, *pTex_);

        // Required for some graphics cards to work with non-power of two images
        // according to http://stackoverflow.com/questions/318194/
        // display-arbitrary-size-2d-image-in-opengl/413658#413658
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        int format = imgAlpha_ ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, imgSize_.x, imgSize_.y, 0, format,
            GL_UNSIGNED_BYTE, pImgData_);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        delete[] pImgData_;
        pImgData_ = 0;
    }
}


const Point&
TextureView::getTextureSize() const
{
    return imgSize_;
}


void
TextureView::renderOGLContent() const
{
    if (pTex_)
    {
        Color4i* shadeCol, *foo;
        shadeColorTable_.queryColors(shadeCol, foo, this);
        setGLColor(shadeCol);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, *pTex_);
        glPushMatrix();
        {
            float m = 1.f / std::min(imgSize_.x, imgSize_.y);
            glScalef(imgSize_.x * m, imgSize_.y * m, 1.f);
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
        }
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
    }
}


const Point&
TextureView::setSize(float width, float height)
{
    // Get pixel-size first
    const Point& newSize = OGLView::setSize(width, height);

    if (!pTex_ && aspectRatioAutoAdapt_ != AR_NO_ADAPT)
    {
        Log::warning("TextureView", Log::os()
            << "No size auto-adaptation due to missing texture");
        return newSize;
    }

    switch (aspectRatioAutoAdapt_)
    {
    case AR_ADAPT_WIDTH:
    {
        if (pTex_)
        {
            float w = static_cast<float>(newSize.y * imgSize_.x) / imgSize_.y;
            return OGLView::setSize(w, newSize.y);
        }
    }
    case AR_ADAPT_HEIGHT:
    {
        if (pTex_)
        {
            float h = static_cast<float>(newSize.x * imgSize_.y) / imgSize_.x;
            return OGLView::setSize(newSize.x, h);
        }
    }
    case AR_NO_ADAPT:
    default:
        break;
    }

    return newSize;
}


} // namespace gw1k

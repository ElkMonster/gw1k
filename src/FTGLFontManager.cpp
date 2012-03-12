/* This code is based on an example implementation provided at
 * http://ftgl.sourceforge.net/docs/html/ftgl-tutorial.html
 */

#include "FTGLFontManager.h"

#include "Log.h"

#include <string>
#include <iostream>

namespace gw1k
{


typedef std::map<std::string, FTFont*> FontList;


/*static*/
FTGLFontManager&
FTGLFontManager::Instance()
{
    static FTGLFontManager tm;
    return tm;
}


FTGLFontManager::~FTGLFontManager()
{
    for(FontList::iterator font = fonts_.begin(); font != fonts_.end(); font++)
    {
        // TODO Find out why delete segfaults; cleanup() is used meanwhile
        //delete (*font).second;
    }

    fonts_.clear();
}


void
FTGLFontManager::cleanup()
{
    for(FontList::iterator font = fonts_.begin(); font != fonts_.end(); font++)
    {
        delete (*font).second;
    }

    fonts_.clear();
}



FTFont*
FTGLFontManager::GetFont(const char *filename, int size)
{
    char buf[256];
    sprintf(buf, "%s%i", filename, size);
    std::string fontKey(buf);

    FontList::iterator result = fonts_.find(fontKey);
    if(result != fonts_.end())
    {
        //Log::info("FTGLFontManager", Log::os() << "Found font " << filename
        //    << " in list");
        return result->second;
    }

    std::string fullname = "fonts/" + std::string(filename);

    // TODO Better use FTBufferFont (for performance)?
    FTFont* font = new FTTextureFont(fullname.c_str());



    if(font->Error() != 0)
    {
        Log::error("FTGLFontManager", Log::os() << "Font " << fullname
            << " failed to open");
        delete font;
        return 0;
    }

    if(!font->FaceSize(size))
    {
        Log::error("FTGLFontManager", Log::os() << "Font " << filename
            << " failed to set size " << size);
        delete font;
        return 0;
    }

    fonts_[fontKey] = font;

    return font;
}


} // namespace gw1k

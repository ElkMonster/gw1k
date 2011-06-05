/* This code is based on an example implementation provided at
 * http://ftgl.sourceforge.net/docs/html/ftgl-tutorial.html
 */

#ifndef GW1K_FTGLFONTMANAGER_H_
#define GW1K_FTGLFONTMANAGER_H_


#include <map>
#include <string>
#include <FTGL/ftgl.h>

namespace gw1k
{


class FTGLFontManager
{

public:

    static FTGLFontManager& Instance();

    ~FTGLFontManager();

    FTFont* GetFont(const char *filename, int size);

    void cleanup();

private:

    // Hide these 'cause this is a singleton.

    FTGLFontManager() {};

    FTGLFontManager(const FTGLFontManager&) {};

    FTGLFontManager& operator=(const FTGLFontManager&) { return *this; };

    // container for fonts
    std::map<std::string, FTFont*> fonts_;

};


} // namespace gw1k

#endif // GW1K_FTGLFONTMANAGER_H_

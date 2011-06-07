#ifndef GW1K_THEMEMANAGER_H_
#define GW1K_THEMEMANAGER_H_

#include "Renderable.h"

extern "C" {
#include <lua.h>
}
#include <map>
#include <string>


namespace gw1k
{


class ThemeManager
{

public:

    static ThemeManager* getInstance();

private:

    ThemeManager(const char* themeName = 0);

    ThemeManager(const ThemeManager&) {}

    ~ThemeManager();

public:

    bool loadTheme(const char* themeName);

    void setColors(const Renderable* r,
                   const char* colorScheme,
                   const char* fallbackScheme);

    const Color4i* getFgColor(const char* colorScheme,
                              const char* fallbackScheme);

    const Color4i* getBgColor(const char* colorScheme,
                              const char* fallbackScheme);

    const Color4i* getHoveredFgColor(const char* colorScheme,
                                     const char* fallbackScheme);

    const Color4i* getHoveredBgColor(const char* colorScheme,
                                     const char* fallbackScheme);

    const Color4i* getClickedFgColor(const char* colorScheme,
                                     const char* fallbackScheme);

    const Color4i* getClickedBgColor(const char* colorScheme,
                                     const char* fallbackScheme);

private:

    const Color4i* getColor(const char* modespec,
                          const char* colorScheme,
                          const char* fallbackScheme);

    bool loadLua();

    void readTheme();

    void luaError();

private:

    static ThemeManager* pInstance_;

    std::map<std::string, Color4i*> colorMap_;

    lua_State* l_;
};


} // namespace

#endif // GW1K_THEMEMANAGER_H_
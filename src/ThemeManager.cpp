#include "ThemeManager.h"
#include "Exception.h"

extern "C" {
#include <lauxlib.h>
#include <lualib.h>
}
#include <iostream>


namespace gw1k
{


/*static*/
ThemeManager* ThemeManager::pInstance_(0);


/*static*/
ThemeManager*
ThemeManager::getInstance()
{
    return pInstance_ ? pInstance_ : (pInstance_ = new ThemeManager());
}


ThemeManager::ThemeManager(const char* themeName)
:   l_(0)
{
    if (!(themeName && loadTheme(themeName)) && !loadTheme("default"))
    {
            throw Exception("Error loading theme; default fallback theme failed, too");
    }
}


ThemeManager::~ThemeManager()
{}


bool
ThemeManager::loadTheme(const char* themeName)
{
    if (!l_ && !loadLua())
    {
        //throw Exception("Error creating Lua state");
        return false;
    }

    int err;

    err = luaL_loadfile(l_, "libs/themes/CreateTheme.lua");
    switch (err)
    {
    case LUA_ERRSYNTAX:
        std::cout << "Lua: Syntax error" << std::endl;
        break;
    case LUA_ERRMEM:
        std::cout << "Lua: Memory allocation error" << std::endl;
        break;
    case LUA_ERRFILE:
        std::cout << "Lua: Could not open CreateTheme.lua" << std::endl;
        break;
    default:
        break;
    }

    lua_pushstring(l_, themeName);
    lua_setglobal(l_, "theme_name");

    err = lua_pcall(l_, 0, 0, 0);
    if (err)
    {
        luaError();
    }

    readTheme();

    return true;
}


void
ThemeManager::setColors(
    Renderable* r,
    const char* colorScheme,
    const char* fallbackScheme)
{
    r->setFgColor(getFgColor(colorScheme, fallbackScheme));
    r->setBgColor(getBgColor(colorScheme, fallbackScheme));
    r->setHoveredFgColor(getHoveredFgColor(colorScheme, fallbackScheme));
    r->setHoveredBgColor(getHoveredBgColor(colorScheme, fallbackScheme));
    r->setClickedFgColor(getClickedFgColor(colorScheme, fallbackScheme));
    r->setClickedBgColor(getClickedBgColor(colorScheme, fallbackScheme));
}


const Color4i*
ThemeManager::getFgColor(
    const char* colorScheme,
    const char* fallbackScheme)
{
    return getColor("fg", colorScheme, fallbackScheme);
}


const Color4i*
ThemeManager::getBgColor(
    const char* colorScheme,
    const char* fallbackScheme)
{
    return getColor("bg", colorScheme, fallbackScheme);
}


const Color4i*
ThemeManager::getHoveredFgColor(
    const char* colorScheme,
    const char* fallbackScheme)
{
    return getColor("hfg", colorScheme, fallbackScheme);
}


const Color4i*
ThemeManager::getHoveredBgColor(
    const char* colorScheme,
    const char* fallbackScheme)
{
    return getColor("hbg", colorScheme, fallbackScheme);
}


const Color4i*
ThemeManager::getClickedFgColor(
    const char* colorScheme,
    const char* fallbackScheme)
{
    return getColor("cfg", colorScheme, fallbackScheme);
}


const Color4i*
ThemeManager::getClickedBgColor(
    const char* colorScheme,
    const char* fallbackScheme)
{
    return getColor("cbg", colorScheme, fallbackScheme);
}


const Color4i*
ThemeManager::getColor(
    const char* modespec,
    const char* colorScheme,
    const char* fallbackScheme)
{
    const char* scheme = (colorScheme ? colorScheme : fallbackScheme);

    if (scheme != 0)
    {
        std::string key(scheme);
        key.append(".").append(modespec);

        std::cout << "ThemeManager: request for key " << key << (colorMap_[key] ? "(success)" : "(fail)") << std::endl;

        return colorMap_[key];
    }
    else
    {
        return 0;
    }
}


bool
ThemeManager::loadLua()
{
    l_ = lua_open();
    luaL_openlibs(l_);

    return true;
}


void
ThemeManager::luaError()
{
    std::cout << "Lua error:\n" << lua_tostring(l_, -1) << std::endl;
}


void
ThemeManager::readTheme()
{
    // Push key-value pairs table onto the stack and read its size
    lua_getglobal(l_, "themeTable");
    int numEntries = lua_objlen(l_, -1);

    // Now traverse all of themeTable's entries
    // Push nil as the first key
    lua_pushnil(l_);

    // lua_next() pops key and pushes new key-value pair; -2 is the index of
    // themeTable, key is at -1; after lua_next(), key is at -3, while -2 and -1
    // contain the new key and value
    while (lua_next(l_, -2) != 0)
    {
        int rgba[4];

        // Top of the stack is a Color object of our Lua Color class, push
        // its rgba field and then all four values
        lua_getfield(l_, -1, "rgba");
        for (int i = 1; i <= 4; ++i)
        {
            // Push index of rgba we want to read, and push and read
            // rgba[index] (gettable() replaces index with value at index)
            lua_pushinteger(l_, i);
            lua_gettable(l_, -2);
            rgba[i - 1] = lua_tointeger(l_, -1);

            // Pop rgba[index] value
            lua_pop(l_, 1);
        }

        // Pop rgba table and current themeTable value
        lua_pop(l_, 2);

        // Read key, but leave it on the stack as the next lua_next() call is
        // going to use it
        std::string key = lua_tostring(l_, -1);

        colorMap_[key] = new Color4i(rgba[0], rgba[1], rgba[2], rgba[3]);
        std::cout << "ThemeManager: Added " << key << " = (" << rgba[0] << ", "
            << rgba[1] << ", " << rgba[2] << ", " << rgba[3] << ")" << std::endl;
    }

    // Finally, pop themeTable
    lua_pop(l_, 1);

}

} // namespace

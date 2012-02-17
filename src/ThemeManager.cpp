#include "ThemeManager.h"

#include "Exception.h"
#include "Log.h"

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


ThemeManager::ThemeManager()
:   l_(0)
{}


ThemeManager::~ThemeManager()
{}


bool
ThemeManager::loadTheme(const char* themeName)
{
    if (!l_ && !loadLua())
    {
         return false;
    }

    int err = luaL_loadfile(l_, "libs/themes/CreateTheme.lua");
    if (err)
    {
        switch (err)
        {
        case LUA_ERRSYNTAX:
            Log::error("ThemeManager", "Lua: Syntax error");
            break;
        case LUA_ERRMEM:
            Log::error("ThemeManager", "Lua: Memory allocation error");
            break;
        case LUA_ERRFILE:
            Log::error("ThemeManager", "Lua: Could not open CreateTheme.lua");
            break;
        default:
            Log::error("ThemeManager", Log::os() << "Lua: Unknown error: " << err);
            break;
        }
        luaError();
        return false;
    }

    lua_pushstring(l_, themeName);
    lua_setglobal(l_, "theme_name");

    err = lua_pcall(l_, 0, 0, 0);
    if (err)
    {
        Log::error("ThemeManager", "There is something wrong in your theme file");
        luaError();
        return false;
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


void
ThemeManager::setColors(
    ColorTable& ct,
    const char* colorScheme,
    const char* fallbackScheme)
{
    setColor(getFgColor(colorScheme, fallbackScheme), ct.fgCol);
    setColor(getBgColor(colorScheme, fallbackScheme), ct.bgCol);
    setColor(getHoveredFgColor(colorScheme, fallbackScheme), ct.hoveredFgCol);
    setColor(getHoveredBgColor(colorScheme, fallbackScheme), ct.hoveredBgCol);
    setColor(getClickedFgColor(colorScheme, fallbackScheme), ct.clickedFgCol);
    setColor(getClickedBgColor(colorScheme, fallbackScheme), ct.clickedBgCol);
}


void
ThemeManager::setColors(Renderable* r, const ColorTable& ct) const
{
    r->setFgColor(ct.fgCol);
    r->setBgColor(ct.bgCol);
    r->setHoveredFgColor(ct.hoveredFgCol);
    r->setHoveredBgColor(ct.hoveredBgCol);
    r->setClickedFgColor(ct.clickedFgCol);
    r->setClickedBgColor(ct.clickedBgCol);
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

        //std::cout << "ThemeManager: request for key " << key << (colorMap_[key] ? "(success)" : "(fail)") << std::endl;

        std::map<std::string, Color4i*>::iterator it = colorMap_.find(key);
        return (it != colorMap_.end()) ? it->second : 0;
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
    Log::error("ThemeManager", Log::os() << "Lua error message:\n"
        << lua_tostring(l_, -1));
    Log::error("ThemeManager", Log::os() <<
        "Check for missing commas at the line numbers given in the error message");
}


void
ThemeManager::readTheme()
{
    // Push key-value pairs table onto the stack and read its size; note that
    // themeTable is a proxy table and _t contains the actual values
    lua_getglobal(l_, "themeTable");
    lua_getfield(l_, -1, "_t");

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
        Log::info("ThemeManager", Log::os() << "Added " << key << " = ("
            << rgba[0] << ", " << rgba[1] << ", " << rgba[2] << ", " << rgba[3]
            << ")");
    }

    // Finally, pop _t and themeTable
    lua_pop(l_, 2);
}


} // namespace

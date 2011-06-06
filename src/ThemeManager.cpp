#include "ThemeManager.h"
#include "Exception.h"


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
{
    if (!loadTheme(themeName) && !loadTheme("default"))
    {
        throw Exception("Error loading theme; default fallback theme failed, too");
    }
}


ThemeManager::~ThemeManager()
{}


bool
ThemeManager::loadTheme(const char* themeName)
{

}


void
ThemeManager::setColors(
    const Renderable* r,
    const char* colorScheme,
    const char* fallbackScheme)
{

}


const Color4i*
ThemeManager::getFgColor(
    const char* colorScheme,
    const char* fallbackScheme)
{
    return getKey("fg", colorScheme, fallbackScheme);
}


const Color4i*
ThemeManager::getBgColor(
    const char* colorScheme,
    const char* fallbackScheme)
{
    return getKey("bg", colorScheme, fallbackScheme);
}


const Color4i*
ThemeManager::getHoveredFgColor(
    const char* colorScheme,
    const char* fallbackScheme)
{
    return getKey("hfg", colorScheme, fallbackScheme);
}


const Color4i*
ThemeManager::getHoveredBgColor(
    const char* colorScheme,
    const char* fallbackScheme)
{
    return getKey("hbg", colorScheme, fallbackScheme);
}


const Color4i*
ThemeManager::getClickedFgColor(
    const char* colorScheme,
    const char* fallbackScheme)
{
    return getKey("cfg", colorScheme, fallbackScheme);
}


const Color4i*
ThemeManager::getClickedBgColor(
    const char* colorScheme,
    const char* fallbackScheme)
{
    return getKey("cbg", colorScheme, fallbackScheme);
}


const Color4i*
ThemeManager::getKey(
    const char* modespec,
    const char* colorScheme,
    const char* fallbackScheme)
{
    const char* scheme = (colorScheme ? colorScheme : fallbackScheme);

    if (scheme != 0)
    {
        std::string key(scheme);
        key.append(".").append(modespec);

        return colorMap_[key];
    }
    else
    {
        return 0;
    }
}


} // namespace

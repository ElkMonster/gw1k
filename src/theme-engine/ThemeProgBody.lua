require("GW1KTheme")
require("Color")

themeTable = {}

function to_color(k, v)

end

-- Take all entries made in the theme file and build the theme table from it
for k,v in pairs(GW1KTheme) do

    if type(k) == "string" and k ~= "_M" and k ~= "_PACKAGE" and k ~= "_NAME" then
        to_color(k, v)
    end

end

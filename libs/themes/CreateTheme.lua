--[[

CreateTheme.lua

This program will read a gw1k theme file, execute it as a chunk of Lua code, and
put all its definitions and results of any statements in a table. That table's
entries are then read and turned into key-value pairs. For the syntax of the
theme file, see default.theme in the themes/ directory. CreateTheme.lua expects
to be called from a base directory with the following structure:

   ./
   |
   +-- themes/
   |        |
   |        +-- default.theme
   |        +-- mytheme.theme
   |        +-- ...
   |
   +-- libs/
          |
          +-- themes/
                   |
                   +-- CreateTheme.lua
                   +-- Color.lua


If anything goes wrong while reading or processing the theme file, or any other
error occurs, this program prints an error message to the command line and
returns false to the caller. Otherwise, it returns true.

]]

require("libs/themes/Color")

-- This variable is normally set by the ThemeManager. Uncomment and set to a
-- theme name in order use this program stand-alone. The given theme is expected
-- to be a file named <theme_name>.theme in the themes/ directory.
local theme_name = "appl"

-- If the global variable CreateTheme_verbose has been set before running this
-- program, it will put out messages about what it is doing. Errors and warnings
-- are printed regardless of verbose's value.
local verbose = CreateTheme_verbose or true

-- This table will hold all of the key-value pairs that can be created after
-- parsing the theme file. _t is the actual container for our data, while
-- themeTable itself acts as a proxy in order to be able to track both
-- insertions as well as updates of key-value pairs. The keys are strings
-- denoting a hierarchy structure ("foo.bar.xy"), the values are Color objects
-- (tables) (see Color.lua)
themeTable = { _t = {} }

setmetatable(themeTable, {

    __newindex = function(t, k, v)
        if verbose then
            local action
            if themeTable._t[k] then
                action = "Warning: " .. ((v and "Updating") or "Deleting")
            else
                action = "New"
            end
            print(action .. " theme table entry: "  .. k .. " = " .. tostring(v))
        end
        themeTable._t[k] = v
    end

})

-- This table will be filled with everything that is defined in the theme file,
-- i.e.
local themeDef = {}

-- Define dummy value for the "no color" setting ("#" in theme file,
-- replaced by "_nocolor" in build_theme_def()), and let it identify itself when
-- it is printed.
_nocolor = {}
setmetatable(_nocolor, { __tostring = function() return "# [_nocolor]" end })


local function log(...)
    if verbose then
        print(...)
    end
end


-- This function reads the theme file, executes it as a Lua code chunk, and
local function build_theme_def()

    log("Reading theme file...")

    -- Redirect all variable definitions made in the theme file to our themeDef
    -- table (instead of putting them in the global environment)
    local mt = {

        -- Variable requests are redirected to the global color table (which
        -- contains definitions for standard colors, see Color.lua), the
        -- themeDef table (in order to be able to reuse definitions within the
        -- theme file), and lastly to our global environment.
        __index = function(t, key)
            return Color.Color[key] or themeDef[key] or rawget(_G, key)
        end,

        -- Any key setting operations are redirected to go into the themeDef
        -- table
        __newindex = function(t, key, value)
            if verbose then
                local action = rawget(themeDef, key) and "Updating" or "New"
                print("themeDef: " .. action .. " entry: ", key, value)
            end
            themeDef[key] = value
        end
    }

    local old_mt = getmetatable(_G)
    setmetatable(_G, mt)

    local theme_file, err_msg = io.open("themes/"..theme_name .. ".theme", "r")

    if theme_file == nil then
        print(err_msg)
        return false
    end

    -- Load theme file, replacing occurrences of "#" by "_nocolor", and execute
    -- the loaded code
    local theme_code = ""
    for line in theme_file:lines() do
        line = string.gsub(line, "#", "_nocolor")
        theme_code = theme_code .. "\n" .. line
    end

    log("Executing theme file...")

    assert(loadstring(theme_code))()

    theme_file:close()

    -- Restore metatable to what it was initially
    setmetatable(_G, old_mt)

    return true
end


-- This table is used in
local selectors = {
    ".fg", ".bg", ".hfg", ".hbg", ".cfg", ".cbg",
    fg = 1, bg = 2, hfg = 3, hbg = 4, cfg = 5, cbg = 6
}


local function is_selector(key)
    return selectors[key]
end


local function is_color(t)
    return (type(t) == "table") and (t.rgba ~= nil)
end


-- Uncomment this to print debugging messages (independently of verbose setting)
--local do_debug = true

local function DEBUGPRINT(...)
    if do_debug then
        print(...)
    end
end


local function print_invalid_value(path, key, value)
    local p = (path and path ~= "" and path .. ".") or ""
    print("Invalid value '" .. tostring(value) .. "' for key '" .. p
        .. tostring(key) .. "'")
end


local function print_invalid_statement(path, key, value)
    local p = (path and path ~= "" and path .. ".") or ""
    print("Invalid statement: " .. p .. tostring(key) .. " = " .. tostring(value))
end


local function print_empty_statement(path, key, value)
    local p = (path and path ~= "" and path .. ".") or ""
    print("Empty statement: " .. p .. tostring(key) .. " = " .. tostring(value)
        .. ", ignoring")
end


local function print_too_many_entries(path, key, value)
    local p = (path and path ~= "" and path .. ".") or ""
    print("Too many entries, skipping: " .. p .. tostring(key) .. " = "
        .. tostring(value))
end


local function ctype(x)
    return x and type(x) == "table" and x.rgba and "color" or type(x)
end


-- Takes all key-value pairs in the themeDef table and turns them into
-- hierarchical strings keys and Color object values (see Color.lua). The keys'
-- hierarchy level is marked by dots between the levels (foo.bar.xy). For each
-- sub-level, the according (sub-)table is processed by calling this function
-- recursively. At base level, any key is allowed, at all other levels, explicit
-- key definitions must either be assigned a color or a table that defines
-- another sub-level. Implicit key definitions (those that are written just as a
-- value inside a table, without a key) must either be assigned a color or a
-- table that contains colors (or another table that contains colors, etc.).
-- Colors can be stated either as a single number, via Color(...), or explicitly
-- via color names. See Color.lua and default.theme for further information.
local function process_theme(path, key, value, level, idx)

    DEBUGPRINT("Entering process_theme()", "path = ", path, "key = ", key, "value = ", value)

    path = path or ""
    level = level or 0
    idx = idx or 1


    if type(key) == "string" then

        DEBUGPRINT(level, "found key ", key, " of type string")

        if type(value) == "number" or is_color(value) or value == _nocolor then

            DEBUGPRINT(level, "found value ", value, " of type " .. ctype(value))

            -- On level 0, allow assignment of arbitrary key - color pairs
            if level == 0 then
                themeTable[key] = Color(value)

            -- Allow direct addressing of widget properties. This will overwrite
            -- a previous setting made for the property
            -- Ex: WiBox { fg = Red }
            elseif is_selector(key) then

                local p = ((path and path ~= "" and path .. ".") or "")

                if value == _nocolor then

                    DEBUGPRINT(level, "table is nocolor")

                    themeTable[p .. key] = nil
                else
                    themeTable[p .. key] = Color(value)
                end

            else
                print_invalid_statement(path, key, value)
            end

        elseif type(value) == "table" then

            DEBUGPRINT(level, "found value ", tostring(value), " of type table")
            DEBUGPRINT(level, "table specifies (sub)widget")

            -- (Sub)Widget specification
            local subidx = 1
            local count = 0;
            for k,v in pairs(value) do
                local p = ((path and path ~= "" and path .. ".") or "")
                subidx = process_theme(p .. key, k, v, level + 1, subidx)
                count = count + 1
            end
            if count == 0 then print_empty_statement(path, key, value) end

        else
            print_invalid_statement(path, key, value)
        end


    elseif type(key) == "number" then

        DEBUGPRINT(level, "found key", key, " of type number")

        if type(value) == "number" then

            DEBUGPRINT(level, "found value ", value, " of type number")

            if idx > 6 then
                print_too_many_entries(path, idx, value)
            else
                themeTable[path .. selectors[idx]] = Color(value)
                idx = idx + 1
            end

        elseif type(value) == "table" then

            DEBUGPRINT(level, "found value ", tostring(value), " of type table")

            if is_color(value) then

                DEBUGPRINT(level, "table is color")

                if idx > 6 then
                    print_too_many_entries(path, idx, value)
                else
                    themeTable[path .. selectors[idx]] = value
                    idx = idx + 1
                end

            elseif value == _nocolor then

                DEBUGPRINT(level, "table is nocolor")
                if idx > 6 then
                    print_too_many_entries(path, idx, value)
                else
                    idx = idx + 1
                end

            else

                DEBUGPRINT(level, "table specifies colors")

                local count = 0;
                for k,v in pairs(value) do
                    idx = process_theme(path, k, v, level, idx)
                    count = count + 1
                end

                if count == 0 then print_empty_statement(path, key, value) end

            end

        else
            print_invalid_statement(path, key, value)
        end

    -- No key given; this should be the case in the initial (level 0) call
    -- (i.e., process_theme(nil, themeDef))
    else

        DEBUGPRINT(level, "found key", tostring(key), " of type " .. type(key))

        if type(value) == "table" then

            DEBUGPRINT(level, "found value ", tostring(value), " of type table")

            for k,v in pairs(value) do
                process_theme(path, k, v)
            end

        else
            print_invalid_value(path, key, value)
        end
    end

    DEBUGPRINT("Leaving process_theme()", "idx = ", idx)
    return idx

end


--------------------------------------------------------------------------------
-- Main Program ----------------------------------------------------------------
--------------------------------------------------------------------------------

log("CreateTheme.lua")
log("Theme file: ", theme_name .. ".theme")

local success = build_theme_def()

log("Processing theme...")

return success and process_theme(nil, nil, themeDef)

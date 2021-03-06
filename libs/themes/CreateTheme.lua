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
returns false to the caller. Otherwise, it returns true. (Note that internal
errors coming from Lua itself will make the program fail.)
Error messages and warnings can also be queried from the C(++) side by reading
the global variable 'CreateTheme_errors'.



]]

-- This variable is normally set by the ThemeManager. If it is not, then the
-- default theme will be loaded. The given theme is expected to be a file named
-- <theme_name>.theme in the themes/ directory.
theme_name = theme_name or "default"

-- If the global variable CreateTheme_verbose has been set before running this
-- program, it will put out messages about what it is doing. Errors and warnings
-- are printed regardless of verbose's value.
local verbose = CreateTheme_verbose or true


local function log(...)
    if verbose then
        print(...)
    end
end


-- This global variable can be read from outside in order to get the list of
-- errors or warnings that occurred during theme processing
CreateTheme_errors = ""


-- Prints the given string and adds it to the CreateTheme_errors variable.
local function log_and_keep(s)
    log(s)
    CreateTheme_errors = CreateTheme_errors .. s .. "\n"
end


local get_theme_file_as_string = function(filename)
    local f, err_msg = io.open(filename, "r")
    if f == nil then
        return nil, err_msg
    end

    local t = { "" }
    local i = 1
    for line in f:lines() do
        t[i] = line
        i = i + 1
    end

    f:close()
    return table.concat(t, "\n"), err_msg
end


log("==== Theme Loader: Starting... =============================")


-- This table will hold all of the key-value pairs that can be created after
-- parsing the theme file. _t is the actual container for our data, while
-- themeTable itself acts as a proxy in order to be able to track both
-- insertions as well as updates of key-value pairs. The keys are strings
-- denoting a hierarchy structure ("foo.bar.xy"), the values are Color objects
-- (tables) (see Color.lua)
themeTable = { _t = {} }

setmetatable(themeTable, {

    __newindex = function(t, k, v)
        local action
        local critical = false
        if themeTable._t[k] then
            critical = true
            action = "Warning: " .. ((v and "Redefinition of") or "Deleting")
        else
            action = "New"
        end

        local s = action .. " theme table entry: "  .. k .. " = " .. tostring(v)
        if critical then
            log_and_keep(s)
        else
            log(s)
        end
        themeTable._t[k] = v
    end

})

-- This table will be filled with everything that is defined in the theme file,
-- i.e. it will contain the raw table structure of the theme.
local themeDef = {}

-- Define dummy value for the "no color" setting ("#" in theme file,
-- replaced by "_nocolor" in build_theme_def()), and let it identify itself when
-- it is printed.
_nocolor = {}
setmetatable(_nocolor, { __tostring = function() return "# [_nocolor]" end })


require("libs/themes/Color")

-- This function reads the theme file, executes it as a Lua code chunk, and
-- makes that all definitions made in the theme file go into the themeTable
-- variable.
local function build_theme_def()

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
            if value == nil then
                log_and_keep("Warning: Assigning undefined value to " .. key)
            end
            local action = (rawget(themeDef, key) and "Updating") or "New"
            log("themeDef: " .. action .. " entry: ", key, value)
            themeDef[key] = value
        end
    }

    local old_mt = getmetatable(_G)
    setmetatable(_G, mt)

    log("Reading gw1k default theme file (default.theme) ...")

    local theme_code, err_msg = get_theme_file_as_string("themes/default.theme")
    if theme_code == nil then
        log(err_msg)
        log("Default theme config file cannot be read or is broken")
        log("==== Theme Loader: Aborted theme processing due to error ===")
        return false
    end

    -- Replace occurrences of "#" by "_nocolor"
    theme_code = string.gsub(theme_code, "#", "_nocolor")

    log("Executing default theme file...")

    assert(loadstring(theme_code))()

    if theme_name ~= "default" then
        log("Reading theme file (" .. theme_name .. ".theme) ...")

        theme_code = nil
        err_msg = nil
        theme_code, err_msg = get_theme_file_as_string("themes/" .. theme_name .. ".theme")
        if theme_code == nil then
            log(err_msg)
            log(theme_name .. ".theme cannot be read or contains broken code")
        else
            -- Replace occurrences of "#" by "_nocolor"
            theme_code = string.gsub(theme_code, "#", "_nocolor")

            log("Executing theme file...")

            assert(loadstring(theme_code))()
        end
    end

    -- Restore metatable to what it was initially
    setmetatable(_G, old_mt)

    return true
end


-- This table is used to get the state selectors for a certain number (e.g., the
-- first color definition in a scheme is the foreground color, so its selector
-- is fg, the fourth color definition is the hovered background (hbg), etc.),
-- and to get the index that is associated with a certain selector. (See
-- process_theme() for details.)
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
    local s = "Invalid value '" .. tostring(value) .. "' for key '" .. p
        .. tostring(key) .. "'"
    log_and_keep(s)
end


local function print_invalid_statement(path, key, value)
    local p = (path and path ~= "" and path .. ".") or ""
    local s = "Invalid statement: " .. p .. tostring(key) .. " = "
        .. tostring(value)
    log_and_keep(s)
end


local function print_empty_statement(path, key, value)
    local p = (path and path ~= "" and path .. ".") or ""
    local s = "Empty statement: " .. p .. tostring(key) .. " = " .. tostring(value)
        .. ", ignoring"
    log_and_keep(s)
end


local function print_too_many_entries(path, key, value)
    local p = (path and path ~= "" and path .. ".") or ""
    local s = "Too many entries, skipping: " .. p .. tostring(key) .. " = "
        .. tostring(value)
    log_and_keep(s)
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

            -- On level 0, allow arbitrary key - color pairs; on all other
            -- levels, allow only direct addressing of widget properties
            -- (fg, bg, hfg, hbg, cfg, cbg). Using direct addressing will
            -- overwrite any previously read setting for the property. Since
            -- the order of keys is undefined for the pairs() iterator, so is
            -- the result for multiple specifications of the same key.
            -- Ex: WiBox { fg = Red }
            if level == 0 or is_selector(key) then
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

        if type(value) == "number" or is_color(value) or value == _nocolor then

            DEBUGPRINT(level, "found value ", value, " of type " .. ctype(value))

            if idx > 6 then
                print_too_many_entries(path, idx, value)
            elseif value == _nocolor then

                DEBUGPRINT(level, "table is nocolor")

                themeTable[path .. selectors[idx]] = nil
            else
                themeTable[path .. selectors[idx]] = Color(value)
            end

            idx = idx + 1

        elseif type(value) == "table" then

            DEBUGPRINT(level, "found value ", tostring(value), " of type table")
            DEBUGPRINT(level, "table specifies colors")

            local count = 0;
            for k,v in pairs(value) do
                idx = process_theme(path, k, v, level, idx)
                count = count + 1
            end

            if count == 0 then print_empty_statement(path, key, value) end

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


log("Theme file: ", theme_name .. ".theme")

local success = build_theme_def()
log("Processing theme...")
success = success and process_theme(nil, nil, themeDef)

log("==== Theme Loader: Done processing theme... ================")

return success

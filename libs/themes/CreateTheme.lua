require("libs/themes/Color")

local theme_name = "appl"


themeTable = { _t = {} }

setmetatable(themeTable, {

    __newindex = function(t, k, v)
        local action
        if themeTable._t[k] then
            action = "Warning: " .. ((v and "Updating") or "Deleting")
        else
            action = "New"
        end
        print(action .. " theme table entry: "  .. k .. " = " .. tostring(v))
        themeTable._t[k] = v
    end,
    
    __index = function(t, k)
        print("__index", k)
        return themeTable._t[k]
    end
})


local themeDef = {}

-- Define dummy value for the "no color" setting ("#" in theme file, 
-- replaced by "_nocolor" in build_theme_def())
_nocolor = {}
setmetatable(_nocolor, { __tostring = function() return "# [_nocolor]" end })


local function build_theme_def()
    
    -- Redirect all variable definitions made in the theme file to our themeDef
    -- table
    local mt = {
        __index = function(t, key)
            return Color.Color[key] or themeDef[key] or rawget(_G, key)
        end,
        __newindex = function(t, key, value)
            local action = rawget(themeDef, key) and "Updating" or "New"
            print("themeDef: " .. action .. " entry: ", key, value)
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
    
    assert(loadstring(theme_code))()
    
    theme_file:close()

    -- Restore metatable to what it was initially
    setmetatable(_G, old_mt)
        
    return true
end


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



local function build_theme_table()
    
end


local do_debug = true

local function DEBUGPRINT(...)
    if do_debug then
        print(unpack(arg))
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

local function f(path, key, value, level, idx)

    DEBUGPRINT("Entering f()", "path = ", path, "key = ", key, "value = ", value)

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
                subidx = f(p .. key, k, v, level + 1, subidx)
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
                    idx = f(path, k, v, level, idx)
                    count = count + 1
                end
                
                if count == 0 then print_empty_statement(path, key, value) end
                
            end
        
        else
            print_invalid_statement(path, key, value)
        end
    
    -- No key given, this should be the case in the initial (level 0) call
    -- (i.e., f(nil, themeDef))
    else
    
        DEBUGPRINT(level, "found key", tostring(key), " of type " .. type(key))
    
        if type(value) == "table" then
  
            DEBUGPRINT(level, "found value ", tostring(value), " of type table")
  
            for k,v in pairs(value) do
                f(path, k, v)
            end
        
        else
            print_invalid_value(path, key, value)
        end
    end

    DEBUGPRINT("Leaving f()", "idx = ", idx)
    return idx
    
end



--------------------------------------------------------------------------------
-- Main Program ----------------------------------------------------------------
--------------------------------------------------------------------------------

return build_theme_def() and f(nil, nil, themeDef) --build_theme_table()

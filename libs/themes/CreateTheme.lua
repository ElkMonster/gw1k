require("libs/themes/Color")

local theme_name = "default"


themeTable = {}

setmetatable(themeTable, {
    __newindex = function(t, k, v)
        local action = t[k] and "Updating" or "New"
        print(action .. " theme table entry: "  .. k .. " = " .. tostring(v))
        rawset(t, k, v);
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


local selectors = { ".fg", ".bg", ".hfg", ".hbg", ".cfg", ".cbg" }


local function is_color(t)
    return t.rgba ~= nil
end



local function build_theme_table()
    
end


--local do_debug = true

local function debugprint(...)
    if do_debug then
        print(unpack(arg))
    end
end


local function print_invalid_statement(path, key, value)
    local p = (path and path ~= "" and path .. ".") or ""
    print("Invalid statement: " .. p .. key .. " = " .. tostring(value))
end


local function print_empty_statement(path, key, value)
    local p = (path and path ~= "" and path .. ".") or ""
    print("Empty statement: " .. p .. key .. " = " .. tostring(value) .. ", ignoring")
end

local function f(path, key, value, level, idx)

    debugprint("Entering f()", "path = ", path, "key = ", key, "value = ", value)

    path = path or ""
    level = level or 0
    idx = idx or 1

    if idx > 6 then
        print("Too many entries, skipping: " .. tostring(key) .. " = " .. tostring(value))
        return idx
    end
    
    if type(key) == "string" then
    
        debugprint(level, "found key ", key, " of type string")
    
        if type(value) == "number" then
        
            debugprint(level, "found value ", value, " of type number")
        
            local p
            if level == 0 then

                themeTable[path .. key] = Color(value)
                
            else
            
                local k = path .. key .. selector[idx]
                local increase_idx = (themeTable[k] == nil)
                themeTable[k] = Color(value)
                if increase_idx then idx = idx + 1 end
                
            end
        
        elseif type(value) == "table" then
            
            debugprint(level, "found value ", tostring(value), " of type table")
            
            if is_color(value) then
            
                debugprint(level, "table is color")
                
                local p
                if level == 0 then 
                
                    themeTable[path .. key] = value
                
                else
                
                    themeTable[path .. key .. selectors[idx]] = value
                    idx = idx + 1
                    
                end
            
            elseif value == _nocolor then
            
                debugprint(level, "table is nocolor")
            
                idx = idx + 1
               
            else
                
                debugprint(level, "table specifies (sub)widget")
                
                -- (Sub)Widget specification
                local subidx = 1
                local count = 0;
                for k,v in pairs(value) do
                    local p = ((path and path ~= "" and path .. ".") or "")
                    subidx = f(p .. key, k, v, level + 1, subidx)
                    count = count + 1
                end
                if count == 0 then print_empty_statement(path, key, value) end
            
            end
        
        else
            print_invalid_statement(path, key, value)            
        end
        
    
    elseif type(key) == "number" then
    
        debugprint(level, "found key", key, " of type number")
    
        if type(value) == "table" then
        
            debugprint(level, "found value ", tostring(value), " of type table")
            
            if is_color(value) then
            
                debugprint(level, "table is color")
                
                themeTable[path .. selectors[idx]] = value
                idx = idx + 1
                
            elseif value == _nocolor then
            
                debugprint(level, "table is nocolor")
            
                idx = idx + 1
            
            else
            
                debugprint(level, "table specifies colors")
                
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
    
        debugprint(level, "found key", tostring(key), " of type " .. type(key))
    
        if type(value) == "table" then
  
            debugprint(level, "found value ", tostring(value), " of type table")
  
            for k,v in pairs(value) do
                f(path, k, v)
            end
        
        else

            local p = ((path and path ~= "" and path .. ".") or "")
            print("Invalid value '" .. tostring(value) .. "' for key '" 
                .. p .. tostring(key) .. "'")
        end
    end

    debugprint("Leaving f()", "idx = ", idx)
    return idx
    
end



--------------------------------------------------------------------------------
-- Main Program ----------------------------------------------------------------
--------------------------------------------------------------------------------

return build_theme_def() and f(nil, nil, themeDef) --build_theme_table()

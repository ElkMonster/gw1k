require("libs/themes/Color")

local theme_name = "default"


themeTable = {}

setmetatable(themeTable, {
    __newindex = function(t, k, v)
        print("New theme table entry: " .. k .. " = " .. tostring(v))
        t[k] = v;
    end
})


local themeDef = {}

-- Define dummy value for the "no color" setting ("#" in theme file, 
-- replaced by "_nocolor" in build_theme_def())
local _nocolor = {}


local function build_theme_def()
    
    -- Redirect all variable definitions made in the theme file to our themeDef
    -- table
    local mt = {
        __index = function(t, key)
            return Color.Color[key] or rawget(_G, key)
        end,
        __newindex = function(t, key, value)
            print("themeDef: new entry: ", key, value)
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
    return t.rgba == nil
end



local function handle_string(str_key, value, idx, level)
    
    idx = idx or 1
    level = level or 0
       
    -- Case: Single int value
    -- Ex: GlobalBackground = 0
    if type(value) == "number" then
        print(level, idx, "Case: Single int value")
        
        local newkey
        if level == 0 then
            newkey = key
        else
            newkey = key .. selectors[idx]
        end
        
        themeTable[newkey] = Color(value)
        
        return idx + 1
        
    -- Case: Table value
    -- Ex: Slider = { ... }
    elseif type(value) == "table" then
    
        -- Case: Color value
        if is_color(value) then
        
            print(level, idx, "Case: Color value")
            
            local newkey
            if level == 0 then
                newkey = key
            else
                newkey = key .. selectors[idx]
            end
            
            themeTable[newkey] = Color(value)
            
            return idx + 1
                    
        -- Case: Non-Color table value
        else
    
    
end

local function to_color(key, value, idx, level)

    idx = idx or 1
    level = level or 0
    
    -- Case: Single int value
    -- Ex: GlobalBackground = 0
    if type(value) == "number" then
        print(level, idx, "Case: Single int value")
        
        local newkey
        if level == 0 then
            newkey = key
        else
            newkey = key .. selectors[idx]
        end
        
        themeTable[newkey] = Color(value)
        
        return idx + 1
        
    -- Case: Table value
    -- Ex: Slider = { ... }
    elseif type(value) == "table" then
    
        -- Case: Color value
        if is_color(value) then
        
            print(level, idx, "Case: Color value")
            
            local newkey
            if level == 0 then
                newkey = key
            else
                newkey = key .. selectors[idx]
            end
            
            themeTable[newkey] = Color(value)
            
            return idx + 1
                    
        -- Case: Non-Color table value
        else
    
            for k,v in pairs(value) do
            
                -- Case: String key
                -- Ex: Slider = { Green, ... } or Slider = { Handle = { ... }, ... }
                if type(k) == "string" then
                
                    -- Found sub-element, handle independently
                    
                    local nidx
                    idx = to_color(key .. k, v, nil, level + 1)
                
                -- Case: Int key
                -- Ex: Slider = { { 255, 127, 0 }, ... } or Slider = { Green, ... }
                -- (Green is an internal table)
                else
                
                end
            
            end
        end
        -- Case: 
        
        --for 
    
    end -- Case: Table

end




local function build_theme_table()
    
    -- Take all entries made in the theme file and build the theme table from it
    for k,v in pairs(themeDef) do
        print(k, v)
        if type(k) == "string" then
            to_color(k, v)
        else
            print("Invalid key (" .. k .. "), ignoring")
        end
    end


end



local function f(path, key, value, level, idx)

    path = path or ""
    level = level or 0
    idx = idx or 1

    if idx > 6 then
        print("Too many entries, skipping: " .. tostring(key) .. " = " .. tostring(value))
        return idx
    end
    
    if type(key) = "string" then
    
        if type(value) == "number" then
        
            if level == 0 then

                themeTable[path .. key] = Color(value)
                
            else
            
                local k = path .. key .. selectors[idx]
                local increase_idx = (themeTable[k] == nil)
                themeTable[k] = Color(value)
                if increase_idx then idx = idx + 1 end
                return idx
                
            end
        
        elseif type(value) == "table" then
            
            if is_color(value) then
            
                themeTable[path .. key .. selectors[idx]] = value
                idx = idx + 1
                return idx

            else
                
                -- (Sub)Widget specification
                for k,v in pairs(value) do
                    local p = ((path and path .. ".") or "")
                    f(p .. "." .. key, k, v, level + 1)
                end
            
            end
        
        else
        
            print("invalid statement: " .. ((path and path .. ".") or "") .. key 
                .. " = " .. tostring(value))
            
        end
        
    
    elseif type(key) == "number" then
    
        if type(value) == "table" then
        
            if is_color(value) then
                themeTable[key .. selectors[idx]] = value
            else
                
                for k,v in pairs(value) do
                    idx = f(path, k, v, level, idx)
                end
                return idx
                
            end
        
        else
        
            print("invalid statement: " .. ((path and path .. ".") or "") .. key 
                .. " = " .. tostring(value))
        
        end
    
    -- No key given, this should be the case in the initial (level 0) call
    -- (i.e., f(nil, themeDef))
    else
    
        if type(value) == "table" then
        
            for k,v in pairs(value) do
                f(k, v)
            end
        
        else
            print("invalid value '" .. tostring(value) .. "' for key '" 
                .. tostring(key) .. "'")
        end
    end
    
end



--------------------------------------------------------------------------------
-- Main Program ----------------------------------------------------------------
--------------------------------------------------------------------------------

return build_theme_def() and build_theme_table()

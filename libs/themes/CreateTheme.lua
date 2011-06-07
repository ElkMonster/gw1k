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


local function build_theme_def()
    
    -- Redirect all variable definitions made in the theme file to our themeDef
    -- table
    local mt = {
        __index = function(t, key)
            return Color.Color[key]
        end,
        __newindex = function(t, key, value)
            themeDef[key] = value
        end
    }
    local old_mt = getmetatable(_G)
    setmetatable(_G, mt)

    -- Define dummy value for the "no color" setting ("#" in theme file, 
    -- replaced by "_nocolor" below)
    local _nocolor = {}
    
    local theme_file, err_msg = io.open("themes/"..theme_name .. ".theme", "r")

    if theme_file == nil then
        print(err_msg)
        return false
    end

    local line_iter_func = theme_file:lines()
    
    -- Load theme file, replacing occurrences of "#" by "_nocolor", and execute
    -- the loaded code
    assert(load(
        function()
            -- Find a non-empty line (load() takes empty strings as the end of 
            -- the code chunk), or EOF
            local line = ""
            while line == "" do
                line = line_iter_func()
                if line == nil then break end
                line = string.gsub(line, "#", "_nocolor")
            end
            print("read: " .. (line or "(EOF)"))
            return line
        end
    ))()
    
    theme_file:close()

    -- Restore metatable to what it was initially
    setmetatable(_G, old_mt)
    
    return true
end


local selectors = { ".fg", ".bg", ".hfg", ".hbg", ".cfg", ".cbg" }


local function to_color(key, value, idx)

    idx = idx or 1
    
    -- Case: Single int value
    -- Ex: GlobalBackground = 0
    if type(value) == "number" then
        print("Case: Single int value")
        themeTable[key .. selectors[idx]] = Color(value)
        return idx + 1
        
    -- Case: Table value
    -- Ex: Slider = { ... }
    elseif type(value) == "table" then
    
        for k,v in pairs(value) do
        
            -- Case: String key
            -- Ex: Slider = { Green, ... } or Slider = { Handle = { ... }, ... }
            if type(k) == "string" then
                --if Color[]
            
                -- Found sub-element, handle independently
                to_color(key .. k, v)
            
            -- Case: Int key
            -- Ex: Slider = { { 255, 127, 0 }, ... } or Slider = { Green, ... }
            -- (Green is an internal table)
            else
            
            end
        
        end
        local number_count = count_numbers(v)
        -- Case: 
        
        --for 
    
    end -- Case: Table

end




local function build_theme_table()

    -- Take all entries made in the theme file and build the theme table from it
    for k,v in pairs(themeDef) do
        print(k,v)
        if type(k) == "string" and k ~= "_M" and k ~= "_PACKAGE" and k ~= "_NAME" 
        then
            to_color(k, v)
        end
    end


end

--------------------------------------------------------------------------------
-- Main Program ----------------------------------------------------------------
--------------------------------------------------------------------------------

return build_theme_def() and build_theme_table()

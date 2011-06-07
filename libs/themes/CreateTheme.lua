local tmp_dir_name = ".themetmp"

local function prepare()

    -- Create a temporary directory for our files.
    -- Afterwards, we assume that directory creation succeeded (or it already 
    -- existed before), so we attempt to create some files in there (i.e., no error 
    -- checks here)
    os.execute("mkdir " .. tmp_dir_name)

    -- This is the file that contains the theme definition; theme_name will be set 
    -- from ThemeManager. The theme file content will be copied into a file that 
    -- creates a Lua module of the theme definition. This module is then loaded by 
    -- the actual theme program in order to create all color key-value pairs.
    local theme_filename = "themes/" .. theme_name .. ".theme"

    --[[ This is the name of the program that will be executed in order to create all 
    -- color key-value pairs
    local theme_prog_filename = tmp_dir_name .. "/CreateTheme.lua"]]

    -- This is the file name of the Lua module that is loaded by theme_prog 
    local theme_module_filename = tmp_dir_name .. "/GW1KTheme.lua"

        
    local theme_file, err_msg = io.open(theme_filename, "r")

    if theme_file == nil then
        print(err_msg)
        return false
    end


    --[[local theme_prog_file, err_msg = io.open(theme_prog_filename)

    if theme_prog_file == nil then
        print(err_msg)
        return false
    end]]


    local theme_module_file, err_msg = io.open(theme_module_filename, "w")

    if theme_module_file == nil then
        print(err_msg)
        return false
    end

    -- Write module file
    theme_module_file:write('require("Color")\n\n')
    theme_module_file:write('module("GW1KTheme")\n\n')
    theme_module_file:write('for k,v in pairs(Color) do')
    theme_module_file:write('    if type(k) == "string" then _G[k] = v\nend')

    theme_module_file:write('module("GW1KTheme")\n\n')
    theme_module_file:write('_nocolor = {}\n\n')
    for line in theme_file:lines() do
        theme_module_file:write(string.gsub(line, "#", "_nocolor")
    end

    theme_module_file:flush()
    theme_module_file:close()
    theme_file:close()

    return true
end -- prepare()


local function count_numbers(t)
    for k,_ in pairs(t)
end

require("Color")
themeTable = {}

local selectors = { ".fg", ".bg", ".hfg", ".hbg", ".cfg", ".cbg" }

local function to_color(key, value, idx)

    idx = idx or 1
    
    -- Case: Single int value
    -- Ex: GlobalBackground = 0
    if type(value) == "number" then
        themeTable[key .. selectors[idx]] = Color(value)
        return idx + 1
        
    -- Case: Table value
    -- Ex: Slider = { ... }
    else if type(value) == "table" then
    
        for k,v in pairs(value) do
        
            -- Case: String key
            -- Ex: Slider = { Green, ... } or Slider = { Handle = { ... }, ... }
            if type(k) == "string" then
                if Color[]
            
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
        
        for 
    
    end -- Case: Table

end




function build_theme_table()

    -- Take all entries made in the theme file and build the theme table from it
    for k,v in pairs(GW1KTheme) do

        if type(k) == "string" and k ~= "_M" and k ~= "_PACKAGE" and k ~= "_NAME" 
        then
            to_color(k, v)
        end
    end


end

--------------------------------------------------------------------------------
-- Main Program ----------------------------------------------------------------
--------------------------------------------------------------------------------



require("GW1KTheme")



return prepare() and build_theme_table()

local theme_name = "default"

local tmp_dir_name = "themetmp"

local function prepare_files()

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

    --
    local theme_module_header_filename = "libs/themes/ThemeModuleHeader.lua"

    -- This is the file name of the Lua module that is loaded by theme_prog 
    local theme_module_filename = tmp_dir_name .. "/GW1KTheme.lua"

        
    local theme_file, err_msg = io.open(theme_filename, "r")

    if theme_file == nil then
        print(err_msg)
        return false
    end


    local theme_module_header_file, err_msg = io.open(theme_module_header_filename)

    if theme_module_header_file == nil then
        print(err_msg)
        return false
    end


    local theme_module_file, err_msg = io.open(theme_module_filename, "w")

    if theme_module_file == nil then
        print(err_msg)
        return false
    end

    -- Write module file
    for line in theme_module_header_file:lines() do
        theme_module_file:write(line .. "\n")
    end
    for line in theme_file:lines() do
        theme_module_file:write(string.gsub(line, "#", "_nocolor") .. "\n")
    end
    theme_module_file:write("\n\n-- Theme File End ---------------------------")
    theme_module_file:write("-----------------------------------\n")

    theme_module_file:flush()
    theme_module_file:close()
    theme_module_header_file:close()
    theme_file:close()

    return true
end -- prepare_files()


local function count_numbers(t)
--    for k,_ in pairs(t)
end

require("libs/themes/Color")
themeTable = {}
setmetatable(themeTable, {
    __newindex = function(t, k, v)
        print("New theme table entry: " .. k .. " = " .. tostring(v))
        t[k] = v;
    end
})
themeDef = {}

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




function build_theme_table()

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

prepare_files()

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

dofile(tmp_dir_name .. "/GW1KTheme.lua")
--require(tmp_dir_name .. "/GW1KTheme")

setmetatable(_G, old_mt)



return build_theme_table()

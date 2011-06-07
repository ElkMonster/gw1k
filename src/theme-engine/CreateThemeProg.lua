-- Create a temporary directory for our files.
-- Afterwards, we assume that directory creation succeeded (or it already 
-- existed before), so we attempt to create some files in there (i.e., no error 
-- checks here)
local tmp_dir_name = ".themetmp"
os.execute("mkdir " .. tmp_dir_name)

-- This is the file that contains the theme definition; theme_name will be set 
-- from ThemeManager. The theme file content will be copied into a file that 
-- creates a Lua module of the theme definition. This module is then loaded by 
-- the actual theme program in order to create all color key-value pairs.
local theme_filename = "themes/" .. theme_name .. ".theme"

-- This is the name of the program that will be executed in order to create all 
-- color key-value pairs
local theme_prog_filename = tmp_dir_name .. "/CreateTheme.lua"

-- This is the file name of the Lua module that is loaded by theme_prog 
local theme_module_filename = tmp_dir_name .. "/GW1KTheme.lua"

	
local theme_file, err_msg = io.open(theme_filename, "r")

if theme_file == nil then
    print(err_msg)
    return
end


local theme_prog_file, err_msg = io.open(theme_prog_filename)

if theme_prog_file == nil then
    print(err_msg)
    return
end


local theme_module_file, err_msg = io.open(theme_module_filename, "w")

if theme_module_file == nil then
    print(err_msg)
    return
end

-- Write module file
theme_module_file:write('module("GW1KTheme")\n')
for line in theme_file:lines() do
    theme_module_file:write(line)
end

theme_module_file:flush()
theme_module_file:close()
theme_file:close()
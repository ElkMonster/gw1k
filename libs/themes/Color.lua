local type = type
local pairs = pairs
local setmetatable = setmetatable
local tostring = tostring
local print = print
local error = error

module("Color")

-- Returns a copy of the given table
local function copytable(t)
    local t2 = {}
    for k,v in pairs(t) do
        t2[k] = v
    end
    return t2
end


-- Returns true if table t only contains numbers, false otherwise
local function only_numbers(t)
    for i = 1,#t do
        if type(t[i]) ~= "number" then
            return false
        end
    end
    return true
end


-- global class object (or table, rather)
Color = {

    mt = {
        
        -- Mark as a metatable, protecting it against changes
        --__metatable = nil,
    
        __index = function(t, key)
            --print("Color: request for key ".. key)
            return Color[key] or t[key]
        end,
        
        -- Prevent messing around with our class
        __newindex = function(t, key, value)
            error("Color does not allow changes to its table structure")
        end,
        
        __tostring = function(c)
            local str = "Color( "
            for i = 1,3 do str = str .. c.rgba[i] .. ", " end
            return  str .. c.rgba[4] .. ")"
        end
    },

    -- Constructor
    new = function(...)
        
        -- This is our new instance; rgba will hold the actual color values
        local obj = { rgba = {} }

        -- Handle nested tables (e.g., Color.new({127, 255, 0}) )
        if #arg == 1 and type(arg[1]) == "table" then
            arg = arg[1]
        end
        
        -- Color.new(127) equals Color.new(127, 127, 127, 255)
        if #arg == 1 and type(arg[1]) == "number" then 
            obj.rgba = { arg[1], arg[1], arg[1], 255 } 
        
        -- Color.new(127, 64) equals Color.new(127, 127, 127, 64)
        elseif #arg == 2 and only_numbers(arg) then
            obj.rgba = { arg[1], arg[1], arg[1], arg[2] }
            
        elseif arg.n == 3 and only_numbers(arg) then
            obj.rgba = copytable(arg)
            obj.rgba[4] = 255
            
        elseif arg.n == 4 and only_numbers(arg) then
            obj.rgba = copytable(arg)
        
        else
            error("Invalid argument ")
            return nil
        end

        --print("Creating Color (a): ", obj)
        setmetatable(obj, Color.mt)
        --print("Creating Color (b): ", obj, ", metatable = ", Color.mt)
        --print("returning new color" .. tostring(obj))
        return obj
    end,
    
    
    -- Creates a new color with the same color, but with a different alpha value
    alpha = function(self, a)
        local newCol = Color.new(self.rgba)
        if a > 0 and a < 1 then
            newCol.rgba[4] = 255 * a
        elseif a >= 0 and a <= 255 then
            newCol.rgba[4] = a
        else
            print("Color: Invalid color value (" .. a .. "), using 255 instead")
            newCol.rgba[4] = 255
        end
        return newCol
    end,


}




Color.Black = Color.new( 0 )
Color.White = Color.new( 255 )
Color.Red = Color.new( 255, 0, 0, 255 )
Color.Green = Color.new( 0, 255, 0 )
Color.Blue = Color.new( 0, 0, 255 )
Color.Yellow = Color.new( 0, 255, 255 )


-- Redirect calls like "Color(255, 0, 0, 255)" to Color.new()
setmetatable(_M, { __call = function(_, ...) return Color.new(...) end })
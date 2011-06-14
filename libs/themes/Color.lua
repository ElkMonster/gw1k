--[[



]]

local type = type
local pairs = pairs
local setmetatable = setmetatable
local tostring = tostring
local print = print
local error = error
local math = math

module("Color")


local function round(v)
    return math.floor(v + 0.5)
end

-- Returns the given value as an appropriate color value from the range [0..255].
-- For arguments in the range (0..1), it returns value * 255; for arguments
-- [0,1..255], it returns the argument; for arguments less than 0 or greater
-- than 255, it returns 0 or 255, respectively.
local function get_color_value(v)
    if v > 0 and v < 1 then
        return round(255 * v)
    elseif v >= 0 and v <= 255 then
        return v
    else
        local w = ((v < 0) and 0) or ((v > 255) and 255)
        print("Color: Invalid color value (" .. w .. "), using 255 instead")
        return w
    end
end


-- Returns a copy of the given table, applying get_color_value to all copied
-- values
local function copytable(t)
    local t2 = {}
    for k,v in pairs(t) do
        t2[k] = get_color_value(v)
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

        __index = function(t, key)
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

        -- Check for copy constructor use
        if #arg == 1 and type(arg[1]) == "table" and arg[1].rgba then
            arg = arg[1].rgba
        end

        -- Color.new(127) equals Color.new(127, 127, 127, 255)
        if #arg == 1 and type(arg[1]) == "number" then
            local n = get_color_value(arg[1])
            obj.rgba = { n, n, n, 255 }

        -- Color.new(127, 64) equals Color.new(127, 127, 127, 64)
        elseif #arg == 2 and only_numbers(arg) then
            local n = get_color_value(arg[1])
            obj.rgba = { n, n, n, get_color_value(arg[2]) }

        elseif #arg == 3 and only_numbers(arg) then
            obj.rgba = copytable(arg)
            obj.rgba[4] = 255

        elseif #arg == 4 and only_numbers(arg) then
            obj.rgba = copytable(arg)

        else
            local m = ""
            for _,v in pairs(arg) do m = m .. " '" .. tostring(v) .. "'" end
            error("Color.new(): Invalid argument(s): " .. (m ~= "" and m) or "<none>")
            return nil
        end

        setmetatable(obj, Color.mt)

        return obj
    end,


    -- Creates a new color with the same color, but with a different alpha value
    alpha = function(self, a)
        local newCol = Color.new(self)
        newCol.rgba[4] = get_color_value(newCol.rgba[4])
        return newCol
    end,


    a = function(self, a)
        return self:alpha(a)
    end

}




Color.Black  = Color.new( 0 )
Color.White  = Color.new( 255 )
Color.Red    = Color.new( 255, 0,   0 )
Color.Green  = Color.new( 0,   255, 0 )
Color.Blue   = Color.new( 0,   0,   255 )
Color.Yellow = Color.new( 0,   255, 255 )
Color.Orange = Color.new( 255, 128, 0 )


-- Redirect calls like "Color(255, 0, 0, 255)" to Color.new()
setmetatable(_M, { __call = function(_, ...) return Color.new(...) end })
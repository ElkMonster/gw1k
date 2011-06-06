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
        if not isnumber(t[i]) then
            return false
        end
    end
    return true
end


-- global class object (or table, rather)
Color = {

    mt = {
        
        -- Mark as a metatable, protecting it against changes
        __metatable = nil,
    
        __index = function(t, key)
            if key == alpha then return t:alpha()
        end,
        
        -- Prevent messing around with our class
        __newindex = function(t, key, value)
            error("Color does not allow changes to its table structure")
        end
    },

    -- Constructor
    new = function(...)
        
        -- This is our new instance; rgba will hold the actual color values
        local obj = { rgba = {} }

        if arg.n == 1 and istable(arg[1]) then
            arg = arg[1]
            arg.n = #arg
        end
        
        -- Color.new(127) equals Color.new(127, 127, 127, 255)
        if arg.n == 1 and isnumber(arg[1]) then 
            obj.rgba = { arg[1], arg[1], arg[1], 255 } 
            
        elseif arg.n == 3 and only_numbers(arg) then
            obj.rgba = copytable(arg)
            obj.rgba[4] = 255
            
        elseif arg.n == 4 and only_numbers(arg) then
            obj.rgba = copytable(arg)
        
        else
            error("Invalid argument ")
            return nil
        end

        setmetatable(obj, mt)
        
        return obj
    end,
    
    
    -- Creates a new color with the same color, but with a different alpha value
    alpha = function(self, a)
        local newCol = Color.new(self.rgba)
        newCol.rgba[4] = a
        return newCol
    end,
    
    


}




Color.Black = Color.new( 0, 0, 0 )
Color.White = Color.new( 255, 255, 255 )
Color.Red = Color.new( 255, 0, 0, 255 )
Color.Green = Color.new( 0, 255, 0 )
Color.Blue = Color.new(  0, 0, 255 )
Color.Yellow = Color.new(  0, 255, 255 )

-- Redirect calls like "Color(255, 0, 0, 255)" to Color.new()
setmetatable(_M, { __call = function(_, ...) return Color.new(...) end })
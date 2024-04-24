comp = {
    new = function(self)
        local newObj = {}
        setmetatable(newObj, { __index = self })
        return newObj
    end,
    start = function(self) end,
    awake = function(self) end,
    initComponent = function(self) end,
    update = function(self) end,
    fixedUpdate = function(self) end,
    render = function(self) end,
    handleEvent = function(self) end,
    -- Con implementacion
    pushEvent = function(self) end,
    object = nil,
    alive = true,
    active = true
}
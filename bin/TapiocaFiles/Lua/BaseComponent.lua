comp = {
    new = function(self)
        local newObj = {}
        setmetatable(newObj, { __index = self })
        return newObj
    end,
    -- Para sobreescribir
    start = function(self) end,
    awake = function(self) end,
    initComponent = function(self) end,
    update = function(self) end,
    fixedUpdate = function(self) end,
    render = function(self) end,
    handleEvent = function(self, id) end,
    -- En cada componente hay estas variables extra
    -- pushEvent = function(self, string id, bool global = true, bool delay = false) -> void
    -- object = GameObject
    -- component = LuaComponent
    -- alive = bool
    -- active = bool
}
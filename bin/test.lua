local serpent = require("serpent")

_internal = {
	showGlobal = { }
}
setmetatable(_G, { __index = _internal.showGlobal })

function _internal.test(tab)
	print(serpent.block(tab))
end

function _internal.testMeta(tab)
	print(serpent.block(getmetatable(tab)))
end

function _internal.call(name, parameters)
	_G[name](table.unpack(parameters))
end
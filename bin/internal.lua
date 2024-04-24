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

function _internal.callGlobal(name, parameters)
	_G[name](table.unpack(parameters))
end

function _internal.call(object, name, parameters)
	object[name](object, table.unpack(parameters))
end

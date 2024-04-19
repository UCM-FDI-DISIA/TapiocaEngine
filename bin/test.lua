local serpent = require("serpent")

_internal = { }

function _internal.test(tab)
	print(serpent.block(tab))
end

function _internal.testMeta(tab)
	print(serpent.block(getmetatable(tab)))
end

function _internal.call(name, parameters)
	_G[name](table.unpack(parameters))
end